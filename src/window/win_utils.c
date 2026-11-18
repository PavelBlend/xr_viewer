#ifndef WIN_UTILS_C
#define WIN_UTILS_C

#define IDI_ICON1 101

#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <windows.h>

#include "..\string.c"

// Развернуть окно на весь экран
void maximize_window(Application* app) {

    if (!app->hwnd || !IsWindow(app->hwnd))
        return;

    // максимизировать окно
    ShowWindow(app->hwnd, SW_MAXIMIZE);

    // Загружаем иконку из файла
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

    if (hIcon) {
        // Устанавливаем иконку для окна
        SendMessage(app->hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
        SendMessage(app->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

}

// Функция изменения размера окна
void reshape(GLFWwindow* window, int w, int h) {
    int side_bar_width = 350;    // ширина боковой панели с параметрами
    glViewport(side_bar_width, 0, w - side_bar_width, h);
}

// Центрировать окно
void center_window(Application* app) {

    GLFWmonitor*        monitor = glfwGetPrimaryMonitor ();
    const GLFWvidmode*  mode    = glfwGetVideoMode      (monitor);

    // Получаем размеры экрана
    int screen_width  = mode->width;
    int screen_height = mode->height;

    // Вычисляем размер окна (80% ширины и высоты экрана)
    int win_size_x = screen_width  * 0.8;
    int win_size_y = screen_height * 0.8;

    // Устанавливаем размер окна
    glfwSetWindowSize(app->window, win_size_x, win_size_y);

    // Вычисляем позицию окна так, чтобы оно было по центру экрана
    int pos_x = (screen_width  - win_size_x) / 2;
    int pos_y = (screen_height - win_size_y) / 2;

    // Установить позицию окна
    glfwSetWindowPos(app->window, pos_x, pos_y);

}

void set_window_transforms(Application* app) {

    const int MAXIMIZE = 1;

    if (MAXIMIZE) {
        // открыть окно на весь экран
        maximize_window(app);

    } else {
        // центрировать окно
        center_window(app);
    }

    // Обновить вьюпорт
    int width, height;
    glfwGetFramebufferSize(app->window, &width, &height);
    reshape(app->window, width, height);

}

// Создать окно
void create_window(Application* app) {

    // Создать строку для заголовка окна
    create_window_title_string(app);

    // создание окна
    app->window = glfwCreateWindow(800, 600, app->window_title, NULL, NULL);
    if (!app->window) {
        glfwTerminate();
        printf("GLFW Window Creation Error!\n");
        exit(-1);
    }

    glfwMakeContextCurrent(app->window);

    // сохраняем нативный дескриптор
    app->hwnd = glfwGetWin32Window(app->window);

}

#endif    // WIN_UTILS_C
