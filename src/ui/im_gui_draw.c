#ifndef IM_GUI_DRAW_C
#define IM_GUI_DRAW_C

#include "cimgui.h"
#include "..\overlays.c"

void update_fps(double* last_time, double* fps) {
    // Получаем текущее время
    double current_time = igGetTime();
    
    // Вычисляем дельту времени между кадрами
    double delta_time = current_time - *last_time;
    *last_time = current_time;
    
    // Предотвращаем деление на ноль
    if (delta_time > 0.0) {
        *fps = 1.0 / delta_time;  // FPS = 1 / время_кадра_в_секундах
    }

}

void draw_ui(RenderContext* ctx, char* buffer, double* fps, double* last_time, uint32_t total_vertices, uint32_t total_indices) {

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    igNewFrame();

    update_fps(last_time, fps);

    igBegin("Info", NULL, 0);    // начать рисовать окно с параметрами

    // Начать рисовать таблицу с параметрами
    if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_None, (ImVec2){0,0}, 0)) {

        igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthFixed,   180.0f, 0);
        igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

        // FPS
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("FPS:");
        igTableSetColumnIndex(1);
        igText("%d", (int)round(*fps));

        // Children Count
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Children Count:");
        igTableSetColumnIndex(1);
        format_uint32_with_spaces(ctx->children_count, buffer);
        igText("%s", buffer);

        // Vertices Count
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Vertices Count:");
        igTableSetColumnIndex(1);
        format_uint32_with_spaces(total_vertices, buffer);
        igText("%s", buffer);

        // Triangles Count
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Triangles Count:");
        igTableSetColumnIndex(1);
        format_uint32_with_spaces(total_indices / 3, buffer);
        igText("%s", buffer);

        // Draw Edges
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Draw Edges:");
        igTableSetColumnIndex(1);
        igCheckbox("##1", &draw_edges);

        // Clear Color
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Clear Color:");
        igTableSetColumnIndex(1);
        igColorEdit3(
            "Clear Color",
            ctx->clear_color,
            ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
        );

        // FOV
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("FOV:");
        igTableSetColumnIndex(1);
        igDragFloat("##2", &fov, 0.5f, 10.0f, 120.0f, "%.1f", 0);

        // Far Plane
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Far Plane:");
        igTableSetColumnIndex(1);
        igDragFloat("##3", &far_plane, 0.5f, 1.0f, 100000.0f, "%.1f", 0);

        // Near Plane
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Near Plane:");
        igTableSetColumnIndex(1);
        igDragFloat("##4", &near_plane, 0.5f, 0.00001f, 1.0f, "%.4f", 0);

        // Закончить рисовать таблицу с параметрами
        igEndTable();

    }

    igEnd();    // закончить рисовать окно с параметрами

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

}

#endif    // IM_GUI_DRAW_C
