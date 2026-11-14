#ifndef APPLICATION_C
#define APPLICATION_C

#include <stdio.h>

#include "application.h"
#include "path_utils.c"
#include "ogf_io.c"
#include "file_formats.h"

// Инициализировать основную структуру программы
void init_app(Application * app) {

    // Вычислить абсолютный путь к exe файлу программы
    get_exe_dir(app);

    // Установить открытый файл по-умолчанию (пустой файл)
    app->file_type   = FILE_NONE;
    app->file        = NULL;
    app->file_path   = NULL;
    app->file_data   = NULL;

}

// Освободить память программы
void free_mem(Application * app) {

    // Освободить память структуры файла
    free_ogf_file(app->file);
    app->file = NULL;

    // Освободить память данных файла
    free_file_data(app->file_data);
    app->file_data = NULL;

}

#endif
