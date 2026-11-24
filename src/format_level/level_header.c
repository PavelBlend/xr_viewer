#ifndef LEVEL_HEADER_C
#define LEVEL_HEADER_C

#include "level_types.h"
#include "level_geom.c"

void read_level_header(
        LevelFile * level_file,
        LevelHeader * header,
        uint8_t * file_bytes,
        Chunks * chunks,
        uint32_t chunk_index,
        char * file_path,
        bool is_level_geom_file
    ) {

    // Текущее смещение
    uint32_t offset = chunks->chunks[chunk_index].offset;

    // Прочитать версию формата
    header->format_version = (uint16_t *) (file_bytes + offset);
    // Пропустить версию формата
    offset += sizeof(uint16_t);

    // Прочитать качество компиляции
    header->quality = (uint16_t *) (file_bytes + offset);
    // Пропустить качество компиляции
    offset += sizeof(uint16_t);

    // Проверка версии формата
    if (*header->format_version != SUPPORTED_LEVEL_VERSION) {
        printf("Unsupported level version: %d\n", *header->format_version);
        exit(EXIT_FAILURE);
    } else {
        printf("Level version: %d\n", *header->format_version);
    }

    // Прочитать level.geom, если версия 14
    if (!is_level_geom_file) {
        read_level_geom_14(level_file, file_path);
    }

}

#endif    // LEVEL_HEADER_C
