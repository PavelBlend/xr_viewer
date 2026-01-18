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

void draw_ui(RenderContext ctx, char* buffer, double fps, double last_time, uint32_t total_vertices, uint32_t total_indices) {

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    igNewFrame();

    update_fps(&last_time, &fps);

    igBegin("Info", NULL, 0);

    igColumns(2, NULL, false);
    igSetColumnWidth(0, 160.0f);     // Ширина для текста

    // Первая колонка
    igText("FPS:");
    igText("Children Count:");
    igText("Vertices Count:");
    igText("Triangles Count:");
    igText("Draw Edges:");
    igText("FOV:");
    igText("Far Plane:");
    igText("Near Plane:");

    igNextColumn();

    // Вторая колонка
    igText("%d", (int)round(fps));

    format_uint32_with_spaces(ctx.children_count, buffer);
    igText("%s", buffer);

    format_uint32_with_spaces(total_vertices, buffer);
    igText("%s", buffer);

    format_uint32_with_spaces(total_indices / 3, buffer);
    igText("%s", buffer);

    igCheckbox("##1", &draw_edges);

    igDragFloat("##2", &fov, 0.5f, 10.0f, 120.0f, "%.1f", 0);
    igDragFloat("##3", &far_plane, 0.5f, 1.0f, 100000.0f, "%.1f", 0);
    igDragFloat("##4", &near_plane, 0.5f, 0.00001f, 1.0f, "%.4f", 0);

    // Вернуться к одной колонке
    igColumns(1, NULL, false);

    igEnd();

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

}

#endif    // IM_GUI_DRAW_C
