#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "..\file_io.c"

#include "..\file_formats.h"
#include "..\render.h"
#include "..\overlays.h"
#include "..\stats\stats_types.h"

typedef struct Application {

    // Работает ли программа или нет
    bool running;

    // GLFW Окно
    GLFWwindow * window;

    // Строка для заголовка окна
    char * window_title;

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

    // Статистика
    Stats stats;

} Application;

Application     create_app  (int argc, char* argv[]);
void            free_mem    (Application * app);

#endif
