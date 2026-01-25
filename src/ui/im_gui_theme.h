#ifndef IM_GUI_THEME_C
#define IM_GUI_THEME_C

#include "cimgui.h"

void cimgui_set_theme(void) {

    ImGuiStyle* style = igGetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = (ImVec4){1.0f, 0.00f, 0.00f, 0.00f};
    colors[ImGuiCol_WindowBg]               = (ImVec4){1.0f, 0.60f, 0.60f, 0.60f};

    colors[ImGuiCol_TitleBg]                = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};
    colors[ImGuiCol_TitleBgActive]          = (ImVec4){1.0f, 0.90f, 0.90f, 0.90f};

    colors[ImGuiCol_FrameBg]                = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};
    colors[ImGuiCol_FrameBgHovered]         = (ImVec4){1.0f, 0.80f, 0.80f, 0.80f};
    colors[ImGuiCol_FrameBgActive]          = (ImVec4){1.0f, 0.90f, 0.90f, 0.90f};

    colors[ImGuiCol_PopupBg]                = (ImVec4){1.0f, 0.60f, 0.60f, 0.60f};

}

#endif    // IM_GUI_THEME_C
