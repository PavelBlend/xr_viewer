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
    printf("\nExecutable file: \"%s\"\n\n", exe_path);
    char *last = strrchr(exe_path, '\\');
    if (last) {
        *(last + 1) = '\0';
    };

    // check cmd arguments
    if (argc < 2) {
        printf("file not specified, command:\n");
        for (int i=0; i<argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        exit(EXIT_FAILURE);
    }

    // get file path
    char* file_path = argv[1];

    // read file
    FileData* file_data = read_file(file_path);
    Chunks* chunks = read_chunks(file_data, 0, (uint32_t)file_data->size);
    OgfFile* ogf_file = read_ogf(file_data, chunks);

    init_free_image();

    // render
    render(ogf_file, argc, argv, exe_path);

    // free memory
    free_file_data(file_data);
    free_chunks(chunks);

    deinit_free_image();

    return 0;
}
