#ifndef IM_GUI_INIT_C
#define IM_GUI_INIT_C

#include "cimgui.h"

#include "im_gui_utils.c"
#include "im_gui_theme.h"

// Инициализация CImGUI
void c_im_gui_init(GLFWwindow* window, char* exe_dir) {

    igCreateContext(NULL);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Установить тему для CImGUI
    cimgui_set_theme();

    // Установить путь к ini файлу настроек
    c_im_gui_set_ini_path(exe_dir);

}

#endif    // IM_GUI_INIT_C
