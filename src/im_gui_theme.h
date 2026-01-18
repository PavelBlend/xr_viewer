#ifndef IM_GUI_THEME_C
#define IM_GUI_THEME_C

#include "cimgui.h"

void cimgui_set_theme(void) {

    ImGuiStyle* style = igGetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = (ImVec4){1.0f, 1.00f, 1.00f, 1.00f};
    colors[ImGuiCol_WindowBg]               = (ImVec4){1.0f, 0.25f, 0.25f, 0.25f};

    colors[ImGuiCol_TitleBg]                = (ImVec4){1.0f, 0.20f, 0.20f, 0.20f};
    colors[ImGuiCol_TitleBgActive]          = (ImVec4){1.0f, 0.30f, 0.30f, 0.30f};

}

#endif    // IM_GUI_THEME_C
