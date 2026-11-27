#pragma once

#include "..\file_io.c"

#include "..\window\window_types.h"
#include "..\file_formats.h"
#include "..\render.h"
#include "..\overlay\overlay_types.h"
#include "..\stats\stats_types.h"
#include "..\prefs\prefs_types.h"

typedef struct Application {

    // Работает ли программа или нет
    bool running;

    // Окно программы
    Window window;

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

    // Контекст рендера
    RenderContext render_ctx;

    // Контекст наложений
    OverlayContext overlay_ctx;

    // Настройки
    Preferences prefs;

    // Статистика
    Stats stats;

} Application;

Application     create_app  (int argc, char* argv[]);
void            free_mem    (Application * app);
