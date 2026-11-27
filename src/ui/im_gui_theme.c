#pragma once

void cimgui_set_theme(void) {

    ImGuiStyle* style = igGetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = (ImVec4){0.00f, 0.00f, 0.00f, 1.00f};
    colors[ImGuiCol_WindowBg]               = (ImVec4){0.60f, 0.60f, 0.60f, 1.00f};

    // Заголовок окна
    colors[ImGuiCol_TitleBg]                = (ImVec4){0.75f, 0.75f, 0.75f, 1.00f};
    colors[ImGuiCol_TitleBgActive]          = (ImVec4){0.90f, 0.90f, 0.90f, 1.00f};

    colors[ImGuiCol_FrameBg]                = (ImVec4){0.75f, 0.75f, 0.75f, 1.00f};
    colors[ImGuiCol_FrameBgHovered]         = (ImVec4){0.80f, 0.80f, 0.80f, 1.00f};
    colors[ImGuiCol_FrameBgActive]          = (ImVec4){0.90f, 0.90f, 0.90f, 1.00f};

    // Фон всплывающих окон
    colors[ImGuiCol_PopupBg]                = (ImVec4){0.75f, 0.75f, 0.75f, 1.00f};

    // Фон главного меню
    colors[ImGuiCol_MenuBarBg]              = (ImVec4){0.75f, 0.75f, 0.75f, 1.00f};

    colors[ImGuiCol_Header]                 = (ImVec4){0.45f, 0.45f, 0.45f, 1.00f};
    colors[ImGuiCol_HeaderHovered]          = (ImVec4){0.50f, 0.50f, 0.50f, 1.00f};
    colors[ImGuiCol_HeaderActive]           = (ImVec4){0.70f, 0.70f, 0.70f, 1.00f};

    // Скролл
    colors[ImGuiCol_ScrollbarBg]            = (ImVec4){0.40f, 0.40f, 0.40f, 1.00f};
    colors[ImGuiCol_ScrollbarGrab]          = (ImVec4){0.55f, 0.55f, 0.55f, 1.00f};
    colors[ImGuiCol_ScrollbarGrabHovered]   = (ImVec4){0.65f, 0.65f, 0.65f, 1.00f};
    colors[ImGuiCol_ScrollbarGrabActive]    = (ImVec4){0.70f, 0.70f, 0.70f, 1.00f};

    // URL
    colors[ImGuiCol_TextLink]               = (ImVec4){0.00f, 0.10f, 0.70f, 1.00f};

    // Скругления
    style->WindowRounding   = 9.0f;
    style->FrameRounding    = 4.0f;

}
