#pragma once

#include "level_types.h"
#include "level_ib.h"
#include "level_reader.c"
#include "..\chunk_io.c"

static void read_one_ib(LevelReader * r, LevelIndexBuffer * ib) {

    ib->count = level_reader_u32(r);
    ib->indices = (uint16_t *) malloc(sizeof(uint16_t) * (size_t) ib->count);

    for (uint32_t i = 0; i < ib->count; i++) {
        ib->indices[i] = level_reader_u16(r);
    }
}

// Освободить один IB
void free_level_index_buffer(LevelIndexBuffer * ib) {

    if (ib == NULL) {
        return;
    }

    free(ib->indices);
    ib->indices = NULL;
    ib->count   = 0;
}

// Чтение всех IB из чанка LEVEL_14_IB
void read_level_ibs(LevelFile * level, FileData * geom_data, Chunks * chunks, uint32_t chunk_index) {

    Chunk * chunk = &chunks->chunks[chunk_index];

    LevelReader r;
    level_reader_init(&r, geom_data->bytes + chunk->offset, *chunk->size);

    uint32_t buffers_count = level_reader_u32(&r);

    printf("IB chunk: buffers_count = %u\n", buffers_count);

    for (uint32_t i = 0; i < buffers_count; i++) {

        LevelIndexBuffer ib;
        memset(&ib, 0, sizeof(ib));

        read_one_ib(&r, &ib);

        printf("IB[%u]: count = %u\n", i, ib.count);

        free_level_index_buffer(&ib);
    }

    if (r.pos != r.size) {
        printf("LEVEL_14_IB chunk: read %u of %u bytes\n", r.pos, r.size);
    }
}
