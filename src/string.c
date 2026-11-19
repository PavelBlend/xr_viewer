#ifndef STRING_C
#define STRING_C

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "encoding_utils.c"
#include "file_io.h"
#include "app\app_types.h"

uint32_t get_string_length(char* string) {
    uint32_t length = 0;

    while (string[length] != 0) {
        length++;
    }

    return length;
}

char* read_string_zero(FileData* file_data, uint32_t* offset) {

    // get string length

    uint32_t offset_zero = *offset;

    while ((uint8_t)*(file_data->bytes + offset_zero) != 0) {
        offset_zero++;
    }

    uint32_t string_length = offset_zero - *offset;

    // read string
    char* string = malloc(string_length + 1);

    for (uint32_t i=0; i<string_length; i++) {
        string[i] = file_data->bytes[*offset + i];
    }
    string[string_length] = 0;

    return string;

}

void create_window_title_string(Application * app) {

    app->window_title = NULL;
    char* window_name = "X-Ray Viewer";

    if (app->file_path != NULL) {

        // путь в UTF-8
        char* file_path_utf8 = ansi_to_utf8(app->file_path);
        if (file_path_utf8 == NULL) {
            file_path_utf8 = _strdup(app->file_path);
        }

        // длина имени программы
        size_t win_name_size = strlen(window_name);

        // длина пути к файлу
        size_t file_path_size = strlen(file_path_utf8);

        // разделители
        char* separator_1 = " - [";
        char* separator_2 = "]";

        // длина разделителей
        size_t separator_1_size = strlen(separator_1);
        size_t separator_2_size = strlen(separator_2);

        // длина имени программы + длина разделителя + длина пути к файлу + длина второго разделителя + 1 нулевой символ
        size_t window_title_size = win_name_size + separator_1_size + file_path_size + separator_2_size + 1;

        // выделить память для строки заголовка окна
        app->window_title = malloc(window_title_size);

        // текущая позиция записи для строки заголовка
        char* pos = app->window_title;

        // скопировать имя программы в итоговую строку
        memcpy(pos, window_name, win_name_size);
        pos += win_name_size;

        // скопировать разделитель в итоговую строку
        memcpy(pos, separator_1, separator_1_size);
        pos += separator_1_size;

        // скопировать путь к файлу в итоговую строку
        memcpy(pos, file_path_utf8, file_path_size);
        pos += file_path_size;

        // скопировать второй разделитель в итоговую строку
        memcpy(pos, separator_2, separator_2_size);
        pos += separator_2_size;

        // добавить нулевой символ
        *pos = 0;

        free(file_path_utf8);

    } else {
        app->window_title = window_name;
    }

}

#endif
