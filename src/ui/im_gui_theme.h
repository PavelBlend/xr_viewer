#ifndef IM_GUI_THEME_C
#define IM_GUI_THEME_C

#include "cimgui.h"

void cimgui_set_theme(void) {

    ImGuiStyle* style = igGetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = (ImVec4){1.0f, 0.00f, 0.00f, 0.00f};
    colors[ImGuiCol_WindowBg]               = (ImVec4){1.0f, 0.60f, 0.60f, 0.60f};

    // Заголовок окна
    colors[ImGuiCol_TitleBg]                = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};
    colors[ImGuiCol_TitleBgActive]          = (ImVec4){1.0f, 0.90f, 0.90f, 0.90f};

    colors[ImGuiCol_FrameBg]                = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};
    colors[ImGuiCol_FrameBgHovered]         = (ImVec4){1.0f, 0.80f, 0.80f, 0.80f};
    colors[ImGuiCol_FrameBgActive]          = (ImVec4){1.0f, 0.90f, 0.90f, 0.90f};

    // Фон всплывающих окон
    colors[ImGuiCol_PopupBg]                = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};

    // Фон главного меню
    colors[ImGuiCol_MenuBarBg]              = (ImVec4){1.0f, 0.75f, 0.75f, 0.75f};

    colors[ImGuiCol_Header]                 = (ImVec4){1.0f, 0.45f, 0.45f, 0.45f};
    colors[ImGuiCol_HeaderHovered]          = (ImVec4){1.0f, 0.50f, 0.50f, 0.50f};
    colors[ImGuiCol_HeaderActive]           = (ImVec4){1.0f, 0.70f, 0.70f, 0.70f};

    // URL
    colors[ImGuiCol_TextLink]               = (ImVec4){1.0f, 0.00f, 0.10f, 0.70f};

}

#endif    // IM_GUI_THEME_C
