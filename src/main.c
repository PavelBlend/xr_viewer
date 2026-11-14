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

    // Вычислить абсолютный путь к exe файлу программы
    get_exe_dir(app);

    // Установить открытый файл по-умолчанию (пустой файл)
    app.file_type   = FILE_NONE;
    app.file        = NULL;
    app.file_path   = NULL;

    // Создать структуры ogf файла
    OgfFile*    ogf_file    = NULL;
    FileData*   file_data   = NULL;
    Chunks*     chunks      = NULL;

    // check cmd arguments
    if (argc < 2) {
        printf("\nfile not specified, current command:\n    ");
        for (int i=0; i<argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n\n");
        printf("\nusage:\n    %s file_name.ogf\n\n", argv[0]);

    } else {

        // get file path
        app.file_path = argv[1];

        // read file
        file_data = read_file(app.file_path);

        // read chunks
        chunks = read_chunks(file_data, 0, (uint32_t)file_data->size);

        // read ogf data
        ogf_file = read_ogf(file_data, chunks);

        // free chunks memory
        free_chunks(chunks);

        // Установить тип файла в ogf
        app.file_type   = FILE_OGF;
        app.file        = ogf_file;

    }

    // render
    render(app.file, argc, argv, app.exe_dir, app.file_path);

    // free file memory
    free_file_data(file_data);

    // free ogf memory
    app.file = NULL;
    free_ogf_file(ogf_file);

    return 0;
}
