#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "types.h"
#include "path_utils.c"

int main (int argc, char* argv[]) {

    // Абсолютный путь к exe файлу программы
    char exe_dir[MAX_PATH];
    int exe_dir_code = get_exe_dir(exe_dir, MAX_PATH);

    if (exe_dir_code != 0) {
        printf("get_exe_dir error: %d\n", exe_dir_code);
        return 1;
    }

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
