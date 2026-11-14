#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>

#include "file_io.c"

#include "file_formats.h"
#include "overlays.h"

typedef struct Application {

    // Работает ли программа или нет
    bool running;

    // Абсолютный путь к exe файлу программы
    char exe_dir[MAX_PATH];

    // Путь к открытому файлу
    char * file_path;

    // Данные файла
    FileData * file_data;

    // Тип файла, который открыт в программе
    FileFormat file_type;

    // Структура, которая хранит данные текущего открытого файла
    void * file;

    // Контекст наложений
    OverlayContext overlay_ctx;

} Application;

Application     create_app  (int argc, char* argv[]);
void            free_mem    (Application * app);

#endif
