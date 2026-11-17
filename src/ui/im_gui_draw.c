#ifndef IM_GUI_DRAW_C
#define IM_GUI_DRAW_C

#include <float.h>
#include <stdbool.h>

#include "cimgui.h"
#include "..\overlays.c"
#include "..\overlays.h"
#include "..\globals.c"
#include "..\render.h"
#include "..\application.h"
#include "..\version.h"

bool show_main_window = true;
bool show_stats_window = true;
bool show_shading_window = true;
bool show_viewport_window = true;
bool show_overlays_window = true;
bool show_navigation_window = true;
bool show_theme_window = true;

float menu_bar_height = 0.0f;

void update_fps(Application* app) {

    // Получаем текущее время
    double current_time = igGetTime();

    // Вычисляем дельту времени между кадрами
    double delta_time = current_time - app->stats.last_time;

    // Устанавливаем предыдущее время
    app->stats.last_time = current_time;

    // Предотвращаем деление на ноль
    if (delta_time > 0.0) {

        // FPS = 1 / время кадра в секундах
        app->stats.fps = 1.0 / delta_time;

    }

}

void draw_main_menu(Application* app) {

    if (igBeginMainMenuBar()) {

        ImVec2 menu_size = igGetWindowSize();
        menu_bar_height = menu_size.y;

        // Меню File
        if (igBeginMenu("File", true)) {

            if (igMenuItemEx("Quit", "", "", false, true)) {
                app->running = false;
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

void draw_ui(Application* app) {

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    igNewFrame();

    RenderContext *  render_ctx  = &app->render_ctx  ;
    OverlayContext * overlay_ctx = &app->overlay_ctx ;

    update_fps(app);

    // Главное меню вверху
    draw_main_menu(app);

    // Получаем указатель на главный вьюпорт
    ImGuiViewport* viewport = igGetMainViewport();

    // Установить позицию окна
    ImVec2 pivot = {0.0f, 0.0f};
    igSetNextWindowPos ((ImVec2){0.0f,   menu_bar_height},      ImGuiCond_Always, pivot);

    // Установить размер окна
    igSetNextWindowSize((ImVec2){350.0f, viewport->WorkSize.y}, ImGuiCond_Always);

    igBegin("Side Bar", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if (igBeginTable("ParamsTable1", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0, 0}, 0)) {

        igTableSetupColumn("Parameter1", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
        igTableSetupColumn("Value1",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

        // версия
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Version:");
        igTableSetColumnIndex(1);
        igText("%d.%d.%d-%s", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX);

        // дата
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Date:");
        igTableSetColumnIndex(1);
        igText("%d-%d-%d", YEAR, MONTH, DAY);

        // автор
        igTableNextRow(0, 0.0f);
        igTableSetColumnIndex(0);
        igText("Author:");
        igTableSetColumnIndex(1);
        igText("%s", AUTHOR);

        igEndTable();

    }

    // URL
    igTextLinkOpenURL(URL, URL);

    igDummy((ImVec2){0, 10});

    // общая строка символов для меток в интерфейсе
    char buffer[15];

    // Рисовать окно со статистикой
    if (igCollapsingHeader_BoolPtr("Stats", &show_stats_window, ImGuiTreeNodeFlags_DefaultOpen)) {

        if (igBeginTable("ParamsTable2", 2, ImGuiTableFlags_SizingStretchSame, (ImVec2){0,0}, 0)) {

            igTableSetupColumn("Parameter2", ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);
            igTableSetupColumn("Value2",     ImGuiTableColumnFlags_WidthStretch, 180.0f, 0);

            // FPS
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("FPS:");
            igTableSetColumnIndex(1);
            igText("%d", (int)round(app->stats.fps));

            // Children Count
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Children Count:");
            igTableSetColumnIndex(1);
            format_uint32_with_spaces(render_ctx->children_count, buffer);
            igText("%s", buffer);

            // Vertices Count
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Vertices Count:");
            igTableSetColumnIndex(1);
            format_uint32_with_spaces(app->stats.total_vertices, buffer);
            igText("%s", buffer);

            // Triangles Count
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Triangles Count:");
            igTableSetColumnIndex(1);
            format_uint32_with_spaces(app->stats.total_indices / 3, buffer);
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
            igCheckbox("##vsync", &render_ctx->vsync);

            // Cull Face
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Cull Face:");
            igTableSetColumnIndex(1);
            igCheckbox("##cull_face", &render_ctx->cull_face);

            // Cull Edge
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Cull Edge:");
            igTableSetColumnIndex(1);
            igCheckbox("##cull_edge", &render_ctx->cull_edge);

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
            igCheckbox("##textures", &render_ctx->use_textures);

            // Shadeless
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Shadeless:");
            igTableSetColumnIndex(1);
            igCheckbox("##shadeless", &render_ctx->shadeless);

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
            igCheckbox("##draw_faces", &overlay_ctx->draw_faces);

            // Draw Edges
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Edges:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_edges", &overlay_ctx->draw_edges);

            // Draw B-Box
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw B-Box:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_bbox", &overlay_ctx->draw_bbox);

            // Draw X-Axis
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw X-Axis:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_x_axis", &overlay_ctx->draw_axis_x);

            // Draw Y-Axis
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Y-Axis:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_y_axis", &overlay_ctx->draw_axis_y);

            // Draw Z-Axis
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Z-Axis:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_z_axis", &overlay_ctx->draw_axis_z);

            // Draw Grid
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Draw Grid:");
            igTableSetColumnIndex(1);
            igCheckbox("##draw_grid", &overlay_ctx->draw_grid);

            // Grid Steps
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Grid Steps:");
            igTableSetColumnIndex(1);
            if (igDragInt("##grid_steps", &overlay_ctx->grid_steps, 1.0f, 1, 1000, "%d", 0)) {
                generate_grid(overlay_ctx);
            }

            // Grid Size
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Grid Size:");
            igTableSetColumnIndex(1);
            if (igDragFloat("##grid_size", &overlay_ctx->grid_size, 1.0f, 0.1f, 100.0f, "%.2f", 0)) {
                generate_grid(overlay_ctx);
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
                render_ctx->clear_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Edges Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Edges Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Edges Color",
                render_ctx->edges_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Grid Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Grid Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Grid Color",
                render_ctx->grid_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // X-Axis Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("X-Axis Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "X-Axis Color",
                render_ctx->x_axis_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Y-Axis Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Y-Axis Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Y-Axis Color",
                render_ctx->y_axis_color,
                ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel
            );

            // Z-Axis Color
            igTableNextRow(0, 0.0f);
            igTableSetColumnIndex(0);
            igText("Z-Axis Color:");
            igTableSetColumnIndex(1);
            igColorEdit3(
                "Z-Axis Color",
                render_ctx->z_axis_color,
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
