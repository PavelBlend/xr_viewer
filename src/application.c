#ifndef APPLICATION_C
#define APPLICATION_C

#include <windows.h>

typedef struct Application {

    // Абсолютный путь к exe файлу программы
    char exe_dir[MAX_PATH];

} Application;

#endif
