#ifndef OGF_IO_C
#define OGF_IO_C

#include <stdlib.h>

#include "ogf_chunks.c"
#include "ogf_verts.c"
#include "ogf_indices.c"
#include "ogf_swis.c"
#include "chunk_io.c"
#include "types.h"

void read_ogf_child(OgfChild* ogf_child, FileData* file_data, Chunks* chunks, int chunk_index) {
    Chunks* chunks_child = read_chunks(
        file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    uint8_t has_swis = 0;

    for (int i=0; i<chunks_child->count; i++) {

        // printf("    id=%d size=%d\n", *chunks_child->chunks[i].id, *chunks_child->chunks[i].size);

        if (*chunks_child->chunks[i].id == VERTICES) {
            read_verts(ogf_child, file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == INDICES) {
            read_indices(ogf_child, file_data, &chunks_child->chunks[i]);

        } else if (*chunks_child->chunks[i].id == SWIDATA) {
            read_swis(ogf_child, file_data, &chunks_child->chunks[i]);
            has_swis = 1;
        }

    }

    if (!has_swis) {
        ogf_child->swis                 = malloc(sizeof(OgfSwis));

        ogf_child->swis->tris_offset    = 0;
        ogf_child->swis->tris_count     = ogf_child->indices->count / 3;
        ogf_child->swis->verts_count   = ogf_child->verts->count;

    }

    // printf("\n");
}

void read_ogf_children(OgfFile* ogf_file, FileData* file_data, Chunks* chunks, int chunk_index) {
    Chunks* chunks_children = read_chunks(
        file_data,
        chunks->chunks[chunk_index].offset,
        chunks->chunks[chunk_index].offset + *chunks->chunks[chunk_index].size
    );

    ogf_file->children = (OgfChild*) malloc(sizeof(OgfChild) * 10);
    ogf_file->children_count = 0;

    for (int visual_id=0; visual_id<chunks_children->count; visual_id++) {
        // printf("  visual_id=%d\n", visual_id);
        read_ogf_child(&ogf_file->children[visual_id], file_data, chunks_children, visual_id);
        ogf_file->children_count++;
    }

}

OgfFile* read_ogf(FileData* file_data, Chunks* chunks) {
    OgfFile* ogf_file = malloc(sizeof(OgfFile));

    for (int i=0; i<chunks->count; i++) {
        // printf("id=%d size=%d\n", *chunks->chunks[i].id, *chunks->chunks[i].size);
        if (*chunks->chunks[i].id == (uint32_t)CHILDREN) {
            read_ogf_children(ogf_file, file_data, chunks, i);
        }
    }

    return ogf_file;
}

#endif    // OGF_IO_C
