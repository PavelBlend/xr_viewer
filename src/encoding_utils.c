#pragma once

// Перевести строку из UTF-16 (wchar_t) в UTF-8.
char* wide_to_utf8(const wchar_t* wide_string) {

    if (wide_string == NULL) {
        return NULL;
    }

    // Узнать размер буфера в байтах (включая завершающий нулевой символ)
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_string, -1, NULL, 0, NULL, NULL);

    if (utf8_size <= 0) {
        return NULL;
    }

    char* utf8_string = (char*) malloc((size_t) utf8_size);

    if (utf8_string == NULL) {
        return NULL;
    }

    if (WideCharToMultiByte(CP_UTF8, 0, wide_string, -1, utf8_string, utf8_size, NULL, NULL) == 0) {
        free(utf8_string);
        return NULL;
    }

    return utf8_string;

}

// Перевести строку из UTF-8 в UTF-16
wchar_t* utf8_to_wide(const char* utf8_string) {

    if (utf8_string == NULL) {
        return NULL;
    }

    // Узнать размер буфера в wchar_t (включая завершающий нулевой символ)
    int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, NULL, 0);

    if (wide_size <= 0) {
        return NULL;
    }

    wchar_t* wide_string = (wchar_t*) malloc(sizeof(wchar_t) * (size_t) wide_size);

    if (wide_string == NULL) {
        return NULL;
    }

    if (MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, wide_string, wide_size) == 0) {
        free(wide_string);
        return NULL;
    }

    return wide_string;

}

// Перевести строку из ANSI кодировки (CP_ACP, на русской Windows это CP1251) в UTF-8.
char* ansi_to_utf8(const char* ansi_string) {

    if (ansi_string == NULL) {
        return NULL;
    }

    // Узнать размер буфера в wchar_t
    int wide_size = MultiByteToWideChar(CP_ACP, 0, ansi_string, -1, NULL, 0);

    if (wide_size <= 0) {
        return NULL;
    }

    wchar_t* wide_string = (wchar_t*) malloc(sizeof(wchar_t) * (size_t) wide_size);

    if (wide_string == NULL) {
        return NULL;
    }

    if (MultiByteToWideChar(CP_ACP, 0, ansi_string, -1, wide_string, wide_size) == 0) {
        free(wide_string);
        return NULL;
    }

    // UTF-16 -> UTF-8
    char* utf8_string = wide_to_utf8(wide_string);

    free(wide_string);

    return utf8_string;

}

// Аналог fopen, который понимает путь в UTF-8
FILE* open_file_utf8(const char* file_path, const wchar_t* mode) {

    wchar_t* wide_path = utf8_to_wide(file_path);

    if (wide_path == NULL) {
        return NULL;
    }

    FILE* file = _wfopen(wide_path, mode);

    free(wide_path);

    return file;

}

// Освободить память аргументов командной строки в UTF-8
void free_utf8_argv(char** argv_utf8, int argc) {

    if (argv_utf8 == NULL) {
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (argv_utf8[i] != NULL) {
            free(argv_utf8[i]);
        }
    }

    free(argv_utf8);

}

// Получить аргументы командной строки в UTF-8
char** get_utf8_argv(int* argc_out) {

    *argc_out = 0;

    // Командная строка Windows в UTF-16
    wchar_t** wide_argv = CommandLineToArgvW(GetCommandLineW(), argc_out);

    if (wide_argv == NULL) {
        return NULL;
    }

    int argc_wide = *argc_out;

    char** argv_utf8 = (char**) malloc(sizeof(char*) * (size_t) argc_wide);

    if (argv_utf8 == NULL) {
        LocalFree(wide_argv);
        *argc_out = 0;
        return NULL;
    }

    // Перевести каждый аргумент в UTF-8
    for (int i = 0; i < argc_wide; i++) {

        argv_utf8[i] = wide_to_utf8(wide_argv[i]);

        // Если хоть один аргумент не сконвертировался - откатиться на обычный argv
        if (argv_utf8[i] == NULL) {
            free_utf8_argv(argv_utf8, i);
            LocalFree(wide_argv);
            *argc_out = 0;
            return NULL;
        }

    }

    // Освободить память, которую выделила CommandLineToArgvW
    LocalFree(wide_argv);

    return argv_utf8;

}
