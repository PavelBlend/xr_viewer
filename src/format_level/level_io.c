#ifndef LEVEL_IO_C
#define LEVEL_IO_C

#include "level_types.h"
#include "level_chunks.h"
#include "level_header.c"
#include "level_shaders.c"
#include "level_visuals.c"
#include "..\chunk_io.c"
#include "..\file_io.c"
#include "..\types.h"
#include "..\file_formats.h"
#include "..\app\app_types.h"

LevelFile* read_level(Application * app, Chunks* chunks) {

    LevelFile * level_file = malloc(sizeof(LevelFile));

    for (uint32_t chunk_index=0; chunk_index<chunks->count; chunk_index++) {

        // Заголовок
        if (*chunks->chunks[chunk_index].id == (uint32_t)LEVEL_HEADER) {
            read_level_header(level_file, &level_file->header, app->file_data->bytes, chunks, chunk_index, app->file_path, false);

        // Шейдеры
        } else if (*chunks->chunks[chunk_index].id == (uint32_t)LEVEL_14_SHADERS) {
            read_level_shaders_14(level_file, app, chunks, chunk_index);

        // Визуалы
        } else if (*chunks->chunks[chunk_index].id == (uint32_t)LEVEL_14_VISUALS) {
            read_level_visuals_14(level_file, app, chunks, chunk_index);

        } else {
            printf("chunk: %u\n", *chunks->chunks[chunk_index].id);
        }

    }

    return level_file;

}

void read_level_file(Application * app, char* argv[]) {

    // Путь к файлу
    app->file_path = argv[1];

    // Прочитать данные файла
    app->file_data = read_file(app->file_path);

    // Создать структуру для чанков level файла
    Chunks* chunks = NULL;

    // Прочитать чанки
    chunks = read_chunks(app->file_data, 0, (uint32_t)app->file_data->size);

    // Прочитать данные level файла
    read_level(app, chunks);
    // app->file = read_level(app, chunks);

    // Освободить память чанков
    free_chunks(chunks);

    // Установить тип файла в level
    // app->file_type = FILE_LEVEL;

}

#endif    // LEVEL_IO_C
