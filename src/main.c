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
        read_ogf_file(&app, argv);
    }

    // render
    render(&app);

    // Освободить память программы
    free_mem(&app);

    return 0;
}
