#ifndef IM_GUI_DRAW__MAIN_MENU_C
#define IM_GUI_DRAW__MAIN_MENU_C

#include "cimgui.h"

#include "..\opengl\opengl_clear.c"
#include "..\app\app.c"
#include "..\app\app_types.h"

void draw_main_menu(Application* app) {

    if (igBeginMainMenuBar()) {

        ImVec2 menu_size = igGetWindowSize();
        menu_bar_height = menu_size.y;

        // Меню File
        if (igBeginMenu("File", true)) {

            if (igMenuItemEx("Clear", "", "", false, true)) {
                // Очистка OpenGL данных
                clear_gl_data(&app->render_ctx);
                // Очистка данных файла
                free_mem(app);
            }

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

#endif    // IM_GUI_DRAW__MAIN_MENU_C
