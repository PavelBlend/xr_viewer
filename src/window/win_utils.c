#ifndef WIN_UTILS_C
#define WIN_UTILS_C

#define IDI_ICON1 101

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

#endif    // WIN_UTILS_C
