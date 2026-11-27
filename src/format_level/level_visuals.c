#pragma once

#include "level_types.h"
#include "..\format_ogf\ogf_chunks.h"
#include "..\format_ogf\ogf_gcontainer.c"
#include "..\chunk_io.c"

void read_level_visual(Application * app, LevelFile * level_file, Chunks *visuals_chunks, uint32_t visual_id) {

    // Прочитать чанки
    Chunks* visual_chunks = read_chunks(
        app->file_data,
        visuals_chunks->chunks[visual_id].offset,
        visuals_chunks->chunks[visual_id].offset + *visuals_chunks->chunks[visual_id].size
    );

    LevelVisual visual;

    for (uint32_t chunk_id=0; chunk_id<visual_chunks->count; chunk_id++) {
        printf("    chunk: %u\n", *visual_chunks->chunks[chunk_id].id);

        // Заголовок
        if (*visual_chunks->chunks[chunk_id].id == HEADER) {
            read_ogf_header(&visual.header, app, visual_chunks, chunk_id);
            printf("        format_version = %hhu\n", *visual.header.format_version);
            printf("        model_type     = %hhu\n", *visual.header.model_type);

        } else if (*visual_chunks->chunks[chunk_id].id == GCONTAINER) {
            read_ogf_gcontainer(&visual.gcontainer, app, visual_chunks, chunk_id);
            printf("        vb_index  = %u\n", *visual.gcontainer.vb_index);
            printf("        vb_offset = %u\n", *visual.gcontainer.vb_offset);
            printf("        vb_size   = %u\n", *visual.gcontainer.vb_size);
            printf("        ib_index  = %u\n", *visual.gcontainer.ib_index);
            printf("        ib_offset = %u\n", *visual.gcontainer.ib_offset);
            printf("        ib_size   = %u\n", *visual.gcontainer.ib_size);
        }

    }

}

void read_level_visuals_14(LevelFile * level_file, Application * app, Chunks * chunks, uint32_t chunk_index) {

    // Прочитать чанки
    Chunks* visuals_chunks = read_chunks(
        app->file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    for (uint32_t visual_id=0; visual_id<visuals_chunks->count; visual_id++) {
        printf("visual: %u\n", visual_id);
        read_level_visual(app, level_file, visuals_chunks, visual_id);
    }

    // Освободить память чанков
    free_chunks(visuals_chunks);

}
