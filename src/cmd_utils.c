#ifndef CMD_UTILS_C
#define CMD_UTILS_C

#include "help.c"
#include "format_ogf\ogf_io.c"

#include "app\app_types.h"

// Получить имя файла из абсолютного пути
// Возвращает указатель на начало имени файла внутри строки path
char * get_filename_from_path(char * path) {

    char * last_sep = strrchr(path, '\\');

    if (last_sep) {
        return last_sep + 1;
    }

    return path;

}

// Извлечь расширение из имени файла
// Возвращает указатель на расширение (без точки) или NULL, если его нет
char * get_file_extension(char * filename) {

    char * dot = strrchr(filename, '.');

    if (!dot || dot == filename) {
        return NULL;
    }

    return dot + 1;

}

// Проверить, что расширение соответствует ogf
int is_ogf_extension(char * extension) {

    if (!extension) {
        return 0;
    }

    return strcmp(extension, "ogf") == 0;

}

// Проверить, что файл имеет расширение ogf, иначе завершить программу
bool ensure_ogf_extension(char * filepath, char * extension) {

    if (!is_ogf_extension(extension)) {
        printf("Incorrect file extension: \".%s\"\n", extension);
        return false;

    } else {
        return true;
    }

}

void read_input_file(Application * app, char * argv[]) {

    char * file_path    = argv[1];
    char * filename     = get_filename_from_path    (file_path);
    char * extension    = get_file_extension        (filename);

    if (extension != NULL) {

        // Чтение *.ogf файла
        if (ensure_ogf_extension(file_path, extension)) {
            read_ogf_file(app, argv);
        }

    } else if (strcmp(filename, "level") == 0) {
        printf("Level files not supported!\n");

    } else {
        printf("Unsupported file: \"%s\"\n", filename);
    }

}

// Обработать аргументы командной строки
void handle_arguments(Application * app, int argc, char * argv[]) {

    if (argc < 2) {
        print_usage(argc, argv);
    } else {
        read_input_file(app, argv);
    }

}

#endif