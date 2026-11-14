#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdlib.h>
#include <inttypes.h>

// структура данных файла
typedef struct FileData {
    uint8_t* bytes;
    size_t size;
} FileData;

#endif    // FILE_IO_H
