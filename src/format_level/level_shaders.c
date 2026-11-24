#ifndef LEVEL_SHADERS_C
#define LEVEL_SHADERS_C

#include "level_types.h"
#include "..\string.c"

void read_level_shaders_14(LevelFile * level_file, Application * app, Chunks * chunks, uint32_t chunk_index) {

    // Текущий чанк
    Chunk * chunk = &chunks->chunks[chunk_index];

    // Текущее смещение
    uint32_t offset = chunk->offset;

    // Прочитать количество шейдеров
    level_file->shaders.count = (uint32_t *) (app->file_data->bytes + offset);
    // Пропустить количество шейдеров
    offset += sizeof(uint32_t);

    char *      shader;
    uint32_t    str_len;

    for (uint32_t shader_id=0; shader_id<*level_file->shaders.count; shader_id++) {

        shader = read_string_zero(app->file_data, &offset);
        printf("\"%s\"\n", shader);

        str_len = get_string_length(shader) + 1;
        offset += str_len;

    }

}

#endif    // LEVEL_SHADERS_C
