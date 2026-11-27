#pragma once

#include "im_gui_utils.c"
#include "im_gui_theme.c"

// Инициализация CImGUI
void c_im_gui_init(Application * app) {

    igCreateContext(NULL);
    ImGui_ImplGlfw_InitForOpenGL(app->window.window_glfw, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    // Установить путь к ini файлу настроек
    c_im_gui_set_ini_path(app->exe_dir);

    // Установить тему для CImGUI
    cimgui_set_theme();

}
