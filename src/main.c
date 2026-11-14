#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "path_utils.c"
#include "application.c"
#include "help.c"

#include "types.h"
#include "file_formats.h"

int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app;

    // Инициализировать основную структуру программы
    init_app(&app);

    // check cmd arguments
    if (argc < 2) {
        print_usage(argc, argv);

    } else {

        // Путь к файлу
        app.file_path = argv[1];

        // Прочитать данные файла
        app.file_data = read_file(app.file_path);

        // Создать структуру для чанков ogf файла
        Chunks* chunks = NULL;

        // Прочитать чанки
        chunks = read_chunks(app.file_data, 0, (uint32_t)app.file_data->size);

        // Прочитать данные ogf файла
        app.file = read_ogf(app.file_data, chunks);

        // Освободить память чанков
        free_chunks(chunks);

        // Установить тип файла в ogf
        app.file_type = FILE_OGF;

    }

    // render
    render(app, argc, argv, app.exe_dir, app.file_path);

    // Освободить память данных файла
    free_file_data(app.file_data);
    app.file_data = NULL;

    // free ogf memory
    free_ogf_file(app.file);
    app.file = NULL;

    return 0;
}
