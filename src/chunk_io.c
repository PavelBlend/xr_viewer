#ifndef CHUNK_IO_C
#define CHUNK_IO_C

#include "file_io.h"
#include "chunk_io.h"
#include "format_ogf\ogf_chunks.h"

// Освободить память чанков
void free_chunks(Chunks* chunked_file) {
    if (chunked_file != NULL) {
        if (chunked_file->chunks != NULL) {
            free(chunked_file->chunks);
        }
        free(chunked_file);
    }
}

// функция для подсчёта количества чанков
uint32_t get_chunks_count(
        FileData* file_data,    // данные всего файла
        uint32_t start,         // начальное смещение внутри данных файла, с которого нужно произвести чтение чанков
        uint32_t end            // конечное смещение внутри файла, на котором нужно закончить считывать чанки
    ) {

    // количество чанков (результат функции)
    uint32_t chunks_count = 0;

    // текущее смещение
    uint32_t offset = start;

    // Текущий чанк
    Chunk chunk;

    // размер заголовка
    uint32_t header_size = (uint32_t)(sizeof(*chunk.id) + sizeof(*chunk.size));

    while (offset < end) {

        // проверить текущее смещение
        if (offset + header_size > end) {
            printf("chunk header error\n");
            exit(EXIT_FAILURE);
        }

        // пропустить id чанка
        offset += sizeof(*chunk.id);

        // прочитать размер чанка
        chunk.size = (uint32_t*) (file_data->bytes + offset);

        // пропустить размер чанка
        offset += sizeof(*chunk.size);

        // проверить текущее смещение
        if (offset + *chunk.size > end) {
            printf("chunk data error\n");
            exit(EXIT_FAILURE);
        }

        // пропустить данные чанка
        offset += *chunk.size;

        // конец данных файла
        if (offset > file_data->size) {
            break;
        }

        chunks_count++;

    }

    return chunks_count;
}

// Прочитать чанки
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
