#ifndef PATH_UTILS_C
#define PATH_UTILS_C

#include <string.h>

int get_exe_dir(char *buffer, DWORD size) {
    // Вычислить абсолютный путь к папке,
    // в которой хранится exe файл программы

    DWORD len = GetModuleFileNameA(NULL, buffer, size);

    if (len == 0)
        return 1;    // ошибка

    if (len == size)
        return 2;    // буфер мал

    // Найти последний слеш в пути
    char *last = strrchr(buffer, '\\');
    if (!last)
        return 3;    // не получилось найти слеш в пути

    // Добавить нулевой байт в коне пути
    *(last + 1) = '\0';

    // Вернуть нулевой код (успешное выполнение)
    return 0;
}

#endif
