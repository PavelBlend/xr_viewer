#ifndef CMD_UTILS_C
#define CMD_UTILS_C

#include "help.c"
#include "format_ogf\ogf_io.c"

#include "app\app_types.h"

// Обработать аргументы командной строки
void handle_arguments(Application * app, int argc, char * argv[]) {

    if (argc < 2) {
        print_usage(argc, argv);
    } else {
        read_ogf_file(app, argv);
    }

}

#endif
