#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "types.h"

int main (int argc, char* argv[]) {

    // Вычислить абсолютный путь к папке,
    // в которой хранится exe файл программы
    char exe_dir[MAX_PATH];
    GetModuleFileNameA(NULL, exe_dir, MAX_PATH);
    char *last = strrchr(exe_dir, '\\');
    if (last) {
        *(last + 1) = '\0';
    };

    OgfFile*    ogf_file    = NULL;
    FileData*   file_data   = NULL;
    char*       file_path   = NULL;
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
        file_path = argv[1];

        // read file
        file_data = read_file(file_path);

        // read chunks
        chunks = read_chunks(file_data, 0, (uint32_t)file_data->size);

        // read ogf data
        ogf_file = read_ogf(file_data, chunks);

        // free chunks memory
        free_chunks(chunks);

    }

    // render
    render(ogf_file, argc, argv, exe_dir, file_path);

    // free file memory
    free_file_data(file_data);

    // free ogf memory
    free_ogf_file(ogf_file);

    return 0;
}
