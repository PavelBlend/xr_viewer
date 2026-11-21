#ifndef RENDER_C
#define RENDER_C

#include "ogf_io.c"
#include "io_fun.c"
#include "globals.c"
#include "tex_io.c"
#include "shaders.c"
#include "overlay\overlay_grid.c"
#include "formating.c"
#include "color_utils.c"

#include "render.h"
#include "types.h"
#include "app\app_types.h"
#include "ogf_verts.h"

#include "ui\im_gui_init.c"
#include "ui\im_gui_deinit.c"
#include "ui\im_gui_draw.c"

#include "render\rend_faces.c"
#include "render\rend_edges.c"
#include "render\rend_bbox.c"
#include "render\rend_grid.c"
#include "render\rend_init.c"
#include "render\rend_settings.c"
#include "overlay\overlay_settings.c"
#include "overlay\overlay_init.c"
#include "window\win_utils.c"
#include "display\display_draw.c"
#include "opengl\opengl_clear.c"
#include "opengl\opengl_settings.c"
#include "glfw\glfw_init.c"
#include "glfw\glfw_deinit.c"
#include "glfw\glfw_callbacks.c"
#include "glew\glew_init.c"
#include "app\app_init.c"
#include "app\app_deinit.c"
#include "app\app_loop.c"

void render(Application * app) {

    // Инициализировать программу
    init_app(app);

    // Инициализировать наложения
    init_overlay(app);

    // Загрузить шейдеры
    load_shaders(app);

    // Инициализировать ресурсы для рендера
    init_render_resources(app);

    // Главный цикл программы
    app_main_loop(app);

    // Деинициализировать программу
    deinit_app(app);

}

#endif    // RENDER_C
