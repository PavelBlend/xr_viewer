#pragma once

#include "file_io.h"

// открыть файл
FILE* open_bin_file(char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("cannot open file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }
    return file;
}

// закрыть файл
void close_file(FILE* file, char* file_path) {
    int close_status = fclose(file);
    if (close_status != 0) {
        printf("cannot close file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }
}

// получить размер файла
size_t get_file_size(FILE* file, char* file_path) {

    int fseek_result;

    fseek_result = fseek(file, 0, SEEK_END);
    if (fseek_result != 0) {
        printf("fseek end error, file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    size_t file_size = (size_t) ftell(file);
    if (file_size == -1) {
        printf("ftell error, file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    fseek_result = fseek(file, 0, SEEK_SET);
    if (fseek_result != 0) {
        printf("fseek set error, file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    return file_size;

}

// прочитать данные файла
FileData* read_file_data(FILE* file, char* file_path) {

    FileData* file_data = (FileData*) malloc(sizeof(FileData));

    file_data->size  =              get_file_size   (file, file_path);
    file_data->bytes = (uint8_t*)   malloc          (file_data->size);

    size_t read_result = fread(file_data->bytes, sizeof(uint8_t), file_data->size / sizeof(uint8_t), file);
    if (read_result != file_data->size) {
        printf("fread error, file: %s, file size: %lld, readed: %lld\n", file_path, file_data->size, read_result);
        exit(EXIT_FAILURE);
    }

    return file_data;

}

// освобожение памяти
void free_file_data(FileData* file_data) {
    if (file_data != NULL) {
        if (file_data->bytes != NULL) {
            free(file_data->bytes);
        }
        free(file_data);
    }
}

// прочитать файл
FileData* read_file(char* file_path) {
    FILE*     file          =   open_bin_file   (file_path      );
    FileData* file_data     =   read_file_data  (file, file_path);
                                close_file      (file, file_path);
    return file_data;
}
