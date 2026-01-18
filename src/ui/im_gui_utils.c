#ifndef IM_GUI_UTILS_C
#define IM_GUI_UTILS_C

#include "cimgui.h"

#define igGetIO igGetIO_Nil

void c_im_gui_set_ini_path(char* exe_dir) {

    // Формируем полный путь к imgui.ini
    static char cimgui_ini_path[MAX_PATH];
    snprintf(cimgui_ini_path, sizeof(cimgui_ini_path), "%s\\imgui.ini", exe_dir);

    // Указать путь к .ini настройкам ImGUI
    ImGuiIO* io = igGetIO();
    io->IniFilename = cimgui_ini_path;

}

#endif    // IM_GUI_UTILS_C
