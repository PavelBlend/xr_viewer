#ifndef RENDER_C
#define RENDER_C

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <windows.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "ogf_io.c"

#include "cimgui.h"
#include "cimgui_impl.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "cglm\cglm.h"
#include "FreeImage.h"

#include "io_fun.c"
#include "globals.c"
#include "tex_io.c"
#include "shaders.c"
#include "overlays.c"
#include "formating.c"
#include "color_utils.c"

#include "render.h"
#include "types.h"
#include "application.h"
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

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Инициализировать наложения
    init_overlay(app);

    // загрузить шейдеры
    render_ctx->ShaderProgram = compile_shaders(app->exe_dir);

    render_ctx->useShadelessLoc = glGetUniformLocation(render_ctx->ShaderProgram, "useShadeless");
    render_ctx->useTextureLoc   = glGetUniformLocation(render_ctx->ShaderProgram, "useTexture");
    render_ctx->solidColorLoc   = glGetUniformLocation(render_ctx->ShaderProgram, "solidColor");
    render_ctx->lightDirLoc     = glGetUniformLocation(render_ctx->ShaderProgram, "lightDir");

    // центр модели
    render_ctx->model_center[0] = 0.0f;
    render_ctx->model_center[1] = 0.0f;
    render_ctx->model_center[2] = 0.0f;

    // Инициализировать ресурсы для рендера
    init_render_resources(app);

    // установить начальную позицию камеры
    pan_x = render_ctx->model_center[0];
    pan_y = render_ctx->model_center[1];
    pan_z = -render_ctx->model_center[2];

    // Главный цикл программы
    app_main_loop(app);

    // Деинициализировать программу
    deinit_app(app);

}

#endif    // RENDER_C
