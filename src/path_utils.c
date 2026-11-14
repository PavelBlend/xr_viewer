#ifndef PATH_UTILS_C
#define PATH_UTILS_C

#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "application.c"

// Напечатать ошибку функции get_exe_dir и закрыть программу
void exe_dir_error(int exe_dir_code) {
    printf("get_exe_dir error: %d\n", exe_dir_code);
    exit(exe_dir_code);
}

// Вычислить абсолютный путь к папке, в которой хранится exe файл программы
void get_exe_dir(Application app) {

    DWORD len = GetModuleFileNameA(NULL, app.exe_dir, MAX_PATH);

    if (len == 0)
        exe_dir_error(1);    // ошибка

    if (len == MAX_PATH)
        exe_dir_error(2);    // буфер мал

    // Найти последний слеш в пути
    char *last = strrchr(app.exe_dir, '\\');
    if (!last)
        exe_dir_error(3);    // не получилось найти слеш в пути

    // Добавить нулевой байт в коне пути
    *(last + 1) = '\0';

}

#endif
