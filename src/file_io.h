#ifndef FILE_IO_H
#define FILE_IO_H

#include "file_io.h"

// структура данных файла
typedef struct FileData {
    uint8_t*    bytes;
    size_t      size;
} FileData;

FILE*       open_bin_file   (char* file_path                );
void        close_file      (FILE* file, char* file_path    );
void        free_file_data  (FileData* file_data            );
size_t      get_file_size   (FILE* file, char* file_path    );
FileData*   read_file_data  (FILE* file, char* file_path    );
FileData*   read_file       (char* file_path                );

#endif    // FILE_IO_H
