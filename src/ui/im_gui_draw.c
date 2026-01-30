#ifndef IM_GUI_DRAW_C
#define IM_GUI_DRAW_C

#include "cimgui.h"
#include "..\overlays.c"
#include "..\render.h"

bool show_stats_window = false;
bool show_viewport_window = false;
bool show_overlays_window = false;

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

void draw_main_menu(void) {

    if (igBeginMainMenuBar()) {

        // Меню File
        if (igBeginMenu("File", true)) {

            if (igMenuItemEx("Quit", "", "", false, true)) {
                running = false;
            }

            igEndMenu();

        }

        // Меню Window
        if (igBeginMenu("Window", true)) {

            if (igMenuItem_Bool("Stats", "", show_stats_window, true)) {
                show_stats_window = !show_stats_window;
            }

            if (igMenuItem_Bool("Viewport", "", show_viewport_window, true)) {
                show_viewport_window = !show_viewport_window;
            }

            if (igMenuItem_Bool("Overlays", "", show_overlays_window, true)) {
                show_overlays_window = !show_overlays_window;
            }

            igEndMenu();

        }

        igEndMainMenuBar();

    }

}

void draw_ui(RenderContext* ctx, char* buffer, double* fps, double* last_time, uint32_t total_vertices, uint32_t total_indices) {

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    igNewFrame();

    update_fps(last_time, fps);

    // Главное меню вверху
    draw_main_menu();

    /*igSetNextWindowSizeConstraints(
        (ImVec2){300, 300},          // минимальный размер окна
        (ImVec2){FLT_MAX, FLT_MAX},   // максимальный размер окна без ограничения
        NULL,
        NULL
    );*/

    // Рисовать таблицу с параметрами
    if (show_stats_window) {

        igBegin("Stats", NULL, 0);    // начать рисовать окно со статистикой

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
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

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }

        igEnd();    // закончить рисовать окно со статистикой
    }

    // Рисовать окно с параметрами вьюпорта
    if (show_viewport_window) {

        igBegin("Viewport", NULL, 0);

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

            // FOV
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("FOV:");
            igTableSetColumnIndex(1);
            igDragFloat("##fov", &fov, 0.5f, 10.0f, 120.0f, "%.1f", 0);

            // Far Plane
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Far Plane:");
            igTableSetColumnIndex(1);
            igDragFloat("##far_plane", &far_plane, 0.5f, 1.0f, 100000.0f, "%.1f", 0);

            // Near Plane
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Near Plane:");
            igTableSetColumnIndex(1);
            igDragFloat("##near_plane", &near_plane, 0.5f, 0.00001f, 1.0f, "%.4f", 0);

            // V-Sync
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("V-Sync:");
            igTableSetColumnIndex(1);
            igCheckbox("##vsync", &vsync);

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }

        igEnd();

    }

    // Рисовать окно с настройками наложений
    if (show_overlays_window) {

        igBegin("Overlays", NULL, 0);

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

            // Draw Faces
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Faces:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_faces", &draw_faces);

            // Draw Edges
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Edges:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_edges", &draw_edges);

            // Draw Grid
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Grid:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_grid", &draw_grid);

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

            // Edges Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Edges Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Edges Color",
                ctx->edges_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Grid Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Grid Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Grid Color",
                ctx->grid_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }

        igEnd();

    }

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

}

#endif    // IM_GUI_DRAW_C
