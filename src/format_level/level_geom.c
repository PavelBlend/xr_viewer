#ifndef LEVEL_GEOM_C
#define LEVEL_GEOM_C

#include "level_types.h"
#include "..\file_io.c"
#include "..\path_utils.c"

// Собрать путь к файлу level.geom в той же папке, что и level файл.
// Возвращает строку, выделенную через malloc
char * build_level_geom_path(const char * level_file_path) {

    if (level_file_path == NULL) {
        return NULL;
    }

    // Найти последний разделитель пути ('\' или '/')
    const char * last_backslash = strrchr(level_file_path, '\\');
    const char * last_slash     = strrchr(level_file_path, '/');

    const char * last_sep = last_backslash;
    if (last_slash != NULL && (last_sep == NULL || last_slash > last_sep)) {
        last_sep = last_slash;
    }

    // Длина папки (включая разделитель). Если разделителя нет — папка пустая.
    size_t dir_len = (last_sep != NULL) ? (size_t)(last_sep - level_file_path + 1) : 0;

    // Имя файла
    const char * geom_name = "level.geom";
    size_t name_len = strlen(geom_name);

    // Выделить память: папка + имя + нулевой байт
    char * path = (char *) malloc(dir_len + name_len + 1);
    if (path == NULL) {
        return NULL;
    }

    // Скопировать папку
    if (dir_len > 0) {
        memcpy(path, level_file_path, dir_len);
    }

    // Скопировать имя файла и добавить нулевой байт
    memcpy(path + dir_len, geom_name, name_len + 1);

    return path;

}

// Проверить, что файл существует (по возможности открыть на чтение)
bool level_geom_exists(const char * geom_path) {

    if (geom_path == NULL) {
        return false;
    }

    FILE * f = fopen(geom_path, "rb");

    if (f == NULL) {
        return false;
    }

    fclose(f);

    return true;

}

// Напечатать ошибку и выйти из программы, если level.geom отсутствует
void ensure_level_geom_exists(const char * geom_path, const char * level_file_path) {

    if (!level_geom_exists(geom_path)) {
        printf(
            "cannot open level.geom: \"%s\" (level file: \"%s\")\n",
            geom_path ? geom_path : "(null)",
            level_file_path ? level_file_path : "(null)"
        );
        exit(EXIT_FAILURE);
    }

}

// Открыть и прочитать level.geom в память.
// Возвращает FileData* — освобождать через free_file_data().
FileData * read_level_geom(const char * level_file_path) {

    // Собрать путь к level.geom
    char * geom_path = build_level_geom_path(level_file_path);
    if (geom_path == NULL) {
        printf("cannot build level.geom path\n");
        exit(EXIT_FAILURE);
    }

    // Убедиться, что файл существует
    ensure_level_geom_exists(geom_path, level_file_path);

    // Прочитать файл
    FileData * geom_data = read_file(geom_path);

    printf("Read Level Geom File: %s\n", geom_path);
    printf("Size Level Geom File: %u\n", (uint32_t)geom_data->size);

    // Освободить временную строку пути
    free(geom_path);

    return geom_data;

}

// Прочитать level.geom
void read_level_geom_14(LevelFile * level, const char * level_file_path) {

    if (*level->header.format_version == 14) {
        level->geom_data = read_level_geom(level_file_path);
    }

    return;

}

#endif    // LEVEL_GEOM_C
