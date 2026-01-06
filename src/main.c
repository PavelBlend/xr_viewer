#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "file_io.c"
#include "chunk_io.c"
#include "ogf_io.c"
#include "render.c"
#include "types.h"
#include "tex_io.c"

int main (int argc, char* argv[]) {

    char exe_path[MAX_PATH];
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    // printf("\nExecutable file: \"%s\"\n\n", exe_path);
    char *last = strrchr(exe_path, '\\');
    if (last) {
        *(last + 1) = '\0';
    };

    OgfFile* ogf_file = NULL;
    FileData* file_data = NULL;
    Chunks* chunks = NULL;

    // check cmd arguments
    if (argc < 2) {
        printf("\nfile not specified, current command:\n    ");
        for (int i=0; i<argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n\n");
        printf("\nusage:\n    %s file_name.ogf\n\n", argv[0]);
        // exit(EXIT_FAILURE);

    } else {
        // get file path
        char* file_path = argv[1];

        // read file
        file_data = read_file(file_path);
        chunks = read_chunks(file_data, 0, (uint32_t)file_data->size);
        ogf_file = read_ogf(file_data, chunks);
    }

    init_free_image();

    // render
    render(ogf_file, argc, argv, exe_path);

    // free memory
    free_file_data(file_data);
    free_chunks(chunks);
    free_ogf_file(ogf_file);

    deinit_free_image();

    return 0;
}
