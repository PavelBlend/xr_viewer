#ifndef OGF_IO_C
#define OGF_IO_C

#include <stdlib.h>

#include "ogf_chunks.h"
#include "ogf_tex.c"
#include "ogf_verts.c"
#include "ogf_indices.c"
#include "ogf_swis.c"
#include "chunk_io.c"
#include "file_io.c"
#include "types.h"
#include "file_formats.h"
#include "app\app_types.h"

void read_child_header(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {
    uint32_t offset = chunk->offset;

    memcpy(&ogf_child->format_version, file_data->bytes + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    offset += sizeof(uint8_t);   // model_type
    offset += sizeof(uint16_t);  // shader_id

    // bbox min
    memcpy(&ogf_child->bbox_min[0], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bbox_min[1], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bbox_min[2], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    // bbox max
    memcpy(&ogf_child->bbox_max[0], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bbox_max[1], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bbox_max[2], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    // bsphere center
    memcpy(&ogf_child->bsphere_center[0], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bsphere_center[1], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_child->bsphere_center[2], file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    // bsphere radius
    memcpy(&ogf_child->bsphere_radius, file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

}

void read_ogf_child(OgfChild* ogf_child, Application* app, Chunks* chunks, int chunk_index) {

    Chunks* chunks_child = read_chunks(
        app->file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    for (int i=0; i<chunks_child->count; i++) {

        if (*chunks_child->chunks[i].id == HEADER) {
            read_child_header(ogf_child, app->file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == TEXTURE) {
            read_texture(ogf_child, app, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == VERTICES) {
            read_verts(ogf_child, app->file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == INDICES) {
            read_indices(ogf_child, app->file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == SWIDATA) {
            read_swis(ogf_child, app->file_data, &chunks_child->chunks[i]);
        }

    }

}

void read_ogf_children(OgfFile* ogf_file, Application* app, Chunks* chunks, int chunk_index) {
    Chunks* chunks_children = read_chunks(
        app->file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    ogf_file->children          = (OgfChild*) malloc(sizeof(OgfChild) * chunks_children->count);
    ogf_file->children_count    = 0;

    for (int visual_id=0; visual_id<chunks_children->count; visual_id++) {
        read_ogf_child(&ogf_file->children[visual_id], app, chunks_children, visual_id);
        ogf_file->children_count++;
    }

}

void read_ogf_header(OgfFile* ogf_file, Application * app, Chunks* chunks, int chunk_index) {
    uint32_t offset = chunks->chunks[chunk_index].offset;

    memcpy(&ogf_file->format_version, app->file_data->bytes + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    offset += sizeof(uint8_t);   // model_type
    offset += sizeof(uint16_t);  // shader_id

    // bbox min
    memcpy(&ogf_file->bbox_min[0], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_file->bbox_min[1], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_file->bbox_min[2], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    // bbox max
    memcpy(&ogf_file->bbox_max[0], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_file->bbox_max[1], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&ogf_file->bbox_max[2], app->file_data->bytes + offset, sizeof(float));
    offset += sizeof(float);

}

// освободить память
void free_ogf_file(OgfFile* ogf_file) {
    if (ogf_file != NULL) {
        if (ogf_file->children != NULL) {
            free(ogf_file->children);
        }
        free(ogf_file);
    }
}

OgfFile* read_ogf(Application * app, Chunks* chunks) {

    OgfFile* ogf_file = malloc(sizeof(OgfFile));

    for (int i=0; i<chunks->count; i++) {

        if (*chunks->chunks[i].id == (uint32_t)HEADER) {
            read_ogf_header(ogf_file, app, chunks, i);

        } else if (*chunks->chunks[i].id == (uint32_t)CHILDREN) {
            read_ogf_children(ogf_file, app, chunks, i);
        }

    }

    return ogf_file;

}

void read_ogf_file(Application * app, char* argv[]) {

    // Путь к файлу
    app->file_path = argv[1];

    // Прочитать данные файла
    app->file_data = read_file(app->file_path);

    // Создать структуру для чанков ogf файла
    Chunks* chunks = NULL;

    // Прочитать чанки
    chunks = read_chunks(app->file_data, 0, (uint32_t)app->file_data->size);

    // Прочитать данные ogf файла
    app->file = read_ogf(app, chunks);

    // Освободить память чанков
    free_chunks(chunks);

    // Установить тип файла в ogf
    app->file_type = FILE_OGF;

}

#endif    // OGF_IO_C
