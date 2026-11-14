#ifndef APPLICATION_H
#define APPLICATION_H

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

void init_app(Application *app);

#endif
