#ifndef APPLICATION_C
#define APPLICATION_C

#include <stdio.h>

#include "application.h"
#include "path_utils.c"
#include "file_formats.h"

// Инициализировать основную структуру программы
void init_app(Application *app) {

    // Вычислить абсолютный путь к exe файлу программы
    get_exe_dir(app);

    // Установить открытый файл по-умолчанию (пустой файл)
    app->file_type   = FILE_NONE;
    app->file        = NULL;
    app->file_path   = NULL;

}

#endif
