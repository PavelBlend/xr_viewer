#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "path_utils.c"
#include "application.c"

#include "types.h"
#include "file_formats.h"


int main (int argc, char* argv[]) {

    // Создать основную структуру программы
    Application app;

    // Инициализировать основную структуру программы
    init_app(&app);

    // Данные файла
    FileData*   file_data   = NULL;

    // check cmd arguments
    if (argc < 2) {
        printf("\nfile not specified, current command:\n    ");
        for (int i=0; i<argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n\n");
        printf("\nusage:\n    %s file_name.ogf\n\n", argv[0]);

    } else {

        // Путь к файлу
        app.file_path = argv[1];

        // Прочитать данные файла
        file_data = read_file(app.file_path);

        // Создать структуру для чанков ogf файла
        Chunks* chunks = NULL;

        // Прочитать чанки
        chunks = read_chunks(file_data, 0, (uint32_t)file_data->size);

        // Прочитать данные ogf файла
        app.file = read_ogf(file_data, chunks);

        // Освободить память чанков
        free_chunks(chunks);

        // Установить тип файла в ogf
        app.file_type = FILE_OGF;

    }

    // render
    render(app, argc, argv, app.exe_dir, app.file_path);

    // Освободить память данных файла
    free_file_data(file_data);

    // free ogf memory
    free_ogf_file(app.file);
    app.file = NULL;

    return 0;
}
