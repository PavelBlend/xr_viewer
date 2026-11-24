#ifndef LEVEL_SWIS_C
#define LEVEL_SWIS_C

#include "level_types.h"
#include "level_swis.h"
#include "level_reader.c"
#include "..\chunk_io.c"

// Прочитать один SWI-буфер
static void import_swi_buffer(LevelReader * r, LevelSWIBuffer * buf) {

    // Пропускаем нулевые uint32, пока не встретим ненулевое — это items_count.
    // В старых ogf перед SWI нет reserved, поэтому цикл должен уметь
    // останавливаться на конце буфера.
    uint32_t reserved    = 0;
    uint32_t items_count = 0;

    while (reserved == 0 && r->pos < r->size) {

        reserved = level_reader_u32(r);

        if (reserved != 0) {
            items_count = reserved;
        }
    }

    buf->items_count = items_count;

    if (items_count == 0) {
        buf->items = NULL;
        return;
    }

    buf->items = (LevelSWIItem *) malloc(sizeof(LevelSWIItem) * (size_t) items_count);

    for (uint32_t i = 0; i < items_count; i++) {

        LevelSWIItem * item = &buf->items[i];

        item->offset          = level_reader_u32(r);
        item->triangles_count = level_reader_u16(r);
        item->vertices_count  = level_reader_u16(r);
    }
}

// Освободить один SWI-буфер
void free_level_swi_buffer(LevelSWIBuffer * buf) {

    if (buf == NULL) {
        return;
    }

    free(buf->items);
    buf->items       = NULL;
    buf->items_count = 0;
}

// Чтение всех SWI из чанка LEVEL_14_SWIS
void read_level_swis(LevelFile * level, FileData * geom_data, Chunks * chunks, uint32_t chunk_index) {

    (void) level;

    Chunk * chunk = &chunks->chunks[chunk_index];

    LevelReader r;
    level_reader_init(&r, geom_data->bytes + chunk->offset, *chunk->size);

    uint32_t buffers_count = level_reader_u32(&r);

    printf("SWIS chunk: buffers_count = %u\n", buffers_count);

    for (uint32_t i = 0; i < buffers_count; i++) {

        LevelSWIBuffer buf;
        memset(&buf, 0, sizeof(buf));

        import_swi_buffer(&r, &buf);

        printf("SWI buffer[%u]: items_count = %u\n", i, buf.items_count);

        for (uint32_t j = 0; j < buf.items_count; j++) {
            const LevelSWIItem * it = &buf.items[j];
            printf(
                "    SWI[%u]: offset = %u, tris = %u, verts = %u\n",
                j, it->offset, it->triangles_count, it->vertices_count
            );
        }

        free_level_swi_buffer(&buf);
    }

    if (r.pos != r.size) {
        printf("LEVEL_14_SWIS chunk: read %u of %u bytes\n", r.pos, r.size);
    }

}

#endif    // LEVEL_SWIS_C
