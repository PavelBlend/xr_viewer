#pragma once

// Инициализировать программу
void init_app(Application * app) {

    // Инициализировать GLFW
    glfw_init();

    // Создать окно
    create_window(app);

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

    // Установка callback'ов GLFW
    set_glfw_callbacks(app);

    // Инициализация CImGUI
    c_im_gui_init(app);

}
