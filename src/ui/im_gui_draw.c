#ifndef IM_GUI_DRAW_C
#define IM_GUI_DRAW_C

#include <float.h>
#include <stdbool.h>

#include "cimgui.h"
#include "..\overlays.c"
#include "..\globals.c"
#include "..\render.h"
#include "..\shading.h"

bool show_main_window = true;
bool show_stats_window = true;
bool show_shading_window = true;
bool show_viewport_window = true;
bool show_overlays_window = true;
bool show_navigation_window = true;
bool show_theme_window = true;

float menu_bar_height = 0.0f;

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

        ImVec2 menu_size = igGetWindowSize();
        menu_bar_height = menu_size.y;

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

            if (igMenuItem_Bool("Shading", "", show_shading_window, true)) {
                show_shading_window = !show_shading_window;
            }

            if (igMenuItem_Bool("Overlays", "", show_overlays_window, true)) {
                show_overlays_window = !show_overlays_window;
            }

            if (igMenuItem_Bool("Theme", "", show_theme_window, true)) {
                show_theme_window = !show_theme_window;
            }

            if (igMenuItem_Bool("Navigation", "", show_navigation_window, true)) {
                show_navigation_window = !show_navigation_window;
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

    // Получаем указатель на главный вьюпорт
    ImGuiViewport* viewport = igGetMainViewport();

    // Установить позицию окна
    ImVec2 pivot = {0.0f, 0.0f};
    igSetNextWindowPos ((ImVec2){0.0f,   menu_bar_height},      ImGuiCond_Always, pivot);

    // Установить размер окна
    igSetNextWindowSize((ImVec2){350.0f, viewport->WorkSize.y}, ImGuiCond_Always);

    igBegin("Side Bar", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // Рисовать окно со статистикой
    if (igCollapsingHeader_BoolPtr("Stats", &show_stats_window, ImGuiTreeNodeFlags_DefaultOpen)) {

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

    }

    // Рисовать окно с параметрами вьюпорта
    if (igCollapsingHeader_BoolPtr("Viewport", &show_viewport_window, ImGuiTreeNodeFlags_DefaultOpen)) {

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
    }

    // Рисовать окно с параметрами для шейдинга
    if (igCollapsingHeader_BoolPtr("Shading", &show_shading_window, ImGuiTreeNodeFlags_DefaultOpen)) {

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

            // Textures
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Textures:");
            igTableSetColumnIndex(1);
            igCheckbox("##textures", &textures);

            // Shadeless
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Shadeless:");
            igTableSetColumnIndex(1);
            igCheckbox("##shadeless", &shadeless);

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }
    }

    // Рисовать окно с настройками наложений
    if (igCollapsingHeader_BoolPtr("Overlays", &show_overlays_window, ImGuiTreeNodeFlags_DefaultOpen)) {

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

            // Grid Steps
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Grid Steps:");
            igTableSetColumnIndex(1);
            if (igDragInt("##grid_steps", &grid_steps, 1.0f, 1, 1000, "%d", 0)) {
                generate_grid(&overlay_ctx);
            }

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }
    }

    // Рисовать окно с цветами темы
    if (igCollapsingHeader_BoolPtr("Theme", &show_theme_window, ImGuiTreeNodeFlags_DefaultOpen)) {

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

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

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }
    }

    // Рисовать окно навигации
    if (igCollapsingHeader_BoolPtr("Navigation", &show_navigation_window, ImGuiTreeNodeFlags_DefaultOpen)) {

        if (igBeginTable("ParamsTable", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

            // Pan X
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Pan X:");
            igTableSetColumnIndex(1);
            igDragFloat("##pan_x", &pan_x, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Pan Y
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Pan Y:");
            igTableSetColumnIndex(1);
            igDragFloat("##pan_y", &pan_y, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Pan Z
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Pan Z:");
            igTableSetColumnIndex(1);
            igDragFloat("##pan_z", &pan_z, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Angle X
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Angle X:");
            igTableSetColumnIndex(1);
            igDragFloat("##camera_angle_x", &camera_angle_x, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Angle Y
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Angle Y:");
            igTableSetColumnIndex(1);
            igDragFloat("##camera_angle_y", &camera_angle_y, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Zoom
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Zoom:");
            igTableSetColumnIndex(1);
            igDragFloat("##zoom", &zoom, 0.01f, -FLT_MAX, FLT_MAX, "%.4f", 0);

            // Закончить рисовать таблицу с параметрами
            igEndTable();

        }
    }

    igEnd();

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

}

#endif    // IM_GUI_DRAW_C
