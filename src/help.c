#pragma once

void print_usage(int argc, char* argv[]) {

    printf("\nfile not specified, current command:\n    ");

    for (int i=0; i<argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n\n");
    printf("\nusage:\n    %s file_name.ogf\n\n", argv[0]);

}
