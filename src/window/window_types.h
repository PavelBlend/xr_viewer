#ifndef WINDOW_TYPES_H
#define WINDOW_TYPES_H

// Структура окна программы
typedef struct Window {

    // GLFW Окно
    GLFWwindow * window_glfw;

    // Строка для заголовка окна
    char * window_title;

    // Нативный дескриптор окна GLFW
    HWND hwnd;

} Window;

#endif    // WINDOW_TYPES_H
