#ifndef LEVEL_HEADER_C
#define LEVEL_HEADER_C

#include "level_types.h"
#include "level_geom.c"

void read_level_header(LevelFile * level, Application * app, Chunks * chunks, uint32_t chunk_index) {

    // Текущее смещение
    uint32_t offset = chunks->chunks[chunk_index].offset;

    // Прочитать версию формата
    level->header.format_version = (uint16_t *) (app->file_data->bytes + offset);
    // Пропустить версию формата
    offset += sizeof(uint16_t);

    // Прочитать качество компиляции
    level->header.quality = (uint16_t *) (app->file_data->bytes + offset);
    // Пропустить качество компиляции
    offset += sizeof(uint16_t);

    // Проверка версии формата
    if (*level->header.format_version != SUPPORTED_LEVEL_VERSION) {
        printf("Unsupported level version: %d\n", *level->header.format_version);
        exit(EXIT_FAILURE);
    } else {
        printf("Level version: %d\n", *level->header.format_version);
    }

    // Прочитать level.geom, если версия 14
    read_level_geom_14(level, app->file_path);

}

#endif    // LEVEL_HEADER_C
