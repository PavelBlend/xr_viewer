#ifndef APP_INIT_C
#define APP_INIT_C

// Инициализировать программу
void init_app(Application * app) {

    // Инициализировать FreeImage
    init_free_image();

    // Инициализировать GLFW
    glfw_init();

    // Создать окно
    create_window(app);

    // Установить позицию и размер окна
    set_window_transforms(app);

    // Инициализировать GLEW
    glew_init();

    // Установить настройки рендера OpenGL
    set_opengl_render_settings();

    // Сгенерировать seed для генератора цвета
    init_color_seed();

    // Установить настройки рендера
    set_render_settings(app);

    // Установить настройки наложений
    set_overlay_settings(app);

}

#endif    // APP_INIT_C
