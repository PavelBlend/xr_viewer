#ifndef LEVEL_SHADERS_C
#define LEVEL_SHADERS_C

#include "level_types.h"
#include "..\string.c"

char * get_shader_diffuse_texture(char * textures) {

    char * texture = NULL;

    if (textures == NULL) {
        return NULL;
    }

    char *comma_pos = strchr(textures, ',');

    if (comma_pos != NULL) {
        // Длина диффузной текстуры (до запятой)
        size_t len = (size_t)(comma_pos - textures);
        // Выделяем память и копируем
        texture = (char *) malloc(len + 1);
        if (texture != NULL) {
            memcpy(texture, textures, len);
            texture[len] = '\0';
        }

    } else {
        // Запятой нет — копируем всю строку
        size_t len = get_string_length(textures);
        texture = (char *) malloc(len + 1);
        if (texture != NULL) {
            memcpy(texture, textures, len + 1);
        }
    }

    return texture;

}

char * get_shader_textures(char * shader) {

    char * textures = NULL;

    // Ищем символ '/' в строке
    char *slash_pos = strchr(shader, '/');

    if (slash_pos != NULL) {
        // Вторая часть после '/'
        textures = slash_pos + 1;

    } else {
        printf("no '/' found, whole shader: \"%s\"\n", shader);
    }

    return textures;

}

void read_level_shader_14(Application * app, uint32_t * offset) {

    uint32_t    str_len;    // длина строки
    char *      shader;     // строка шейдера

    // Прочитать строку шейдера
    shader = read_string_zero(app->file_data, offset);

    // Вычислить длину строки (+1 - нулевой байт в конце)
    str_len = get_string_length(shader) + 1;

    // Прибавить смещение
    *offset += str_len;

    // Относительный путь к диффузной текстуре
    char * texture;

    if (str_len > 1) {

        // Извлечь пути к текстурам
        char * textures = get_shader_textures(shader);

        // Извлечь относительный путь к диффузной текстуре
        texture = get_shader_diffuse_texture(textures);

    } else {
        // Пустой шейдер
        texture = "\0";
    }

    printf("\"%s\"\n", texture);

}

void read_level_shaders_14(LevelFile * level_file, Application * app, Chunks * chunks, uint32_t chunk_index) {

    // Текущий чанк
    Chunk * chunk = &chunks->chunks[chunk_index];

    // Текущее смещение
    uint32_t offset = chunk->offset;

    // Прочитать количество шейдеров
    level_file->shaders.count = (uint32_t *) (app->file_data->bytes + offset);
    // Пропустить количество шейдеров
    offset += sizeof(uint32_t);

    // Чтение шейдеров
    for (uint32_t shader_id=0; shader_id<*level_file->shaders.count; shader_id++) {
        read_level_shader_14(app, &offset);
    }

}

#endif    // LEVEL_SHADERS_C
