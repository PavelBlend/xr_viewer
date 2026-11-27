#pragma once

// Структура окна программы
typedef struct Window {

    // GLFW Окно
    GLFWwindow * window_glfw;

    // Строка для заголовка окна
    char * window_title;

    // Нативный дескриптор окна GLFW
    HWND hwnd;

} Window;
