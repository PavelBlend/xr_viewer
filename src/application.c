#ifndef APPLICATION_C
#define APPLICATION_C

#include <stdbool.h>

#include "application.h"
#include "file_formats.h"

#include "path_utils.c"
#include "cmd_utils.c"
#include "ogf_io.c"

// Создать основную структуру программы
Application create_app(int argc, char* argv[]) {

    Application app;

    // Программа запущена
    app.running = true;

    // Вычислить абсолютный путь к exe файлу программы
    get_exe_dir(&app);

    // Установить открытый файл по-умолчанию (пустой файл)
    app.file_type   = FILE_NONE;
    app.file        = NULL;
    app.file_path   = NULL;
    app.file_data   = NULL;

    // Обработать аргументы командной строки
    handle_arguments(&app, argc, argv);

    return app;

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
