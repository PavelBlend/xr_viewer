#ifndef WIN_UTILS_C
#define WIN_UTILS_C

#define IDI_ICON1 101

#include <stdio.h>

#include "..\string.c"

void maximizeWindow(Application* app) {

    // Получаем handle окна
    HWND hwnd = FindWindow(NULL, app->window_title);

    if (hwnd) {
        // максимизировать окно
        ShowWindow(hwnd, SW_MAXIMIZE);

        // Загружаем иконку из файла
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

        if (hIcon) {
            // Устанавливаем иконку для окна
            SendMessage(hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }

    }

}

void reshape(GLFWwindow* window, int w, int h) {
    int side_bar_width = 350;    // ширина боковой панели с параметрами
    glViewport(side_bar_width, 0, w - side_bar_width, h);
}

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

}

#endif    // WIN_UTILS_C
