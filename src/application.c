#ifndef APPLICATION_C
#define APPLICATION_C

#include <windows.h>

#include "file_formats.h"

typedef struct Application {

    // Абсолютный путь к exe файлу программы
    char exe_dir[MAX_PATH];

    // Путь к открытому файлу
    char * file_path;

    // Тип файла, который открыт в программе
    FileFormat file_type;

    // Структура, которая хранит данные текущего открытого файла
    void * file;

} Application;

#endif
