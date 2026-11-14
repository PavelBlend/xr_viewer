#ifndef CHUNK_IO_C
#define CHUNK_IO_C

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "file_io.h"

#include "ogf_chunks.c"

typedef struct Chunk {
    uint32_t* id;
    uint32_t* size;
    uint32_t offset;
} Chunk;

typedef struct Chunks {
    uint32_t count;
    Chunk* chunks;
} Chunks;

void free_chunks(Chunks* chunked_file) {
    if (chunked_file != NULL) {
        free(chunked_file->chunks);
        free(chunked_file);
    }
}

uint32_t get_chunks_count(FileData* file_data, uint32_t start, uint32_t end) {

    // количество чанков
    uint32_t chunks_count = 0;

    // начальное смещение
    uint32_t offset = start;

    Chunk* chunk = malloc(sizeof(Chunk));

    while (offset < end) {

        // размер заголовка
        uint32_t header_size = (uint32_t)(sizeof(chunk->id) + sizeof(chunk->size));

        // проверить текущее смещение
        uint32_t unreaded = (end - start) - (offset - start);
        if (header_size > unreaded) {
            printf("incorrect file size: %lld\n", file_data->size);
            exit(EXIT_FAILURE);
        }

        // пропустить id чанка
        offset += sizeof(*chunk->id);

        // размер чанка
        uint32_t* chunk_size = (uint32_t*) (file_data->bytes + offset);
        offset += sizeof(*chunk->size);

        offset += *chunk_size;

        // конец данных файла
        if (offset > file_data->size) {
            break;
        }

        chunks_count++;

    }

    free(chunk);

    return chunks_count;
}

Chunks* read_chunks(FileData* file_data, uint32_t start, uint32_t end) {

    // текущее смещение в данных файла
    uint32_t offset = start;

    // выделить память под чанки
    Chunks* chunks = malloc(sizeof(Chunks));
    chunks->count = get_chunks_count(file_data, start, end);
    chunks->chunks = malloc(sizeof(Chunk) * chunks->count);

    // прочитать чанки
    for (int i=0; i<chunks->count; i++) {

        // id чанка
        chunks->chunks[i].id = (uint32_t*) &file_data->bytes[offset];
        offset += sizeof(*chunks->chunks[i].id);

        // размер чанка
        chunks->chunks[i].size = (uint32_t*) &file_data->bytes[offset];
        offset += sizeof(*chunks->chunks[i].size);

        // указать смещение данных для текущего чанка
        chunks->chunks[i].offset = offset;

        // пропустить данные
        offset += *chunks->chunks[i].size;

    }

    return chunks;

}

#endif    // CHUNK_IO_C
