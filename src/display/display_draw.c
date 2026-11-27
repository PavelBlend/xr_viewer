#ifndef DISPLAY_DRAW_C
#define DISPLAY_DRAW_C

#include "display_clear.c"
#include "display_vsync.c"

#include "..\light\light_utils.c"
#include "..\render\rend_faces.c"
#include "..\render\rend_edges.c"
#include "..\render\rend_points.c"
#include "..\render\rend_bbox.c"
#include "..\render\rend_nav_center.c"
#include "..\render\rend_grid.c"
#include "..\shaders.c"

void display_draw(GLFWwindow* window_glfw, Application* app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    // Установить вертикальную синхронизацию
    set_vsync(render_ctx);

    // Очистить экран
    display_clear(render_ctx);

    // Использовать шейдерную программу
    glUseProgram(render_ctx->ShaderProgram);

    // Установить матрицы для шейдера
    set_shader_matrices(window_glfw, render_ctx->ShaderProgram, render_ctx->ShaderProgramNavCenter);

    // Добавить источник света
    add_light(window_glfw, render_ctx);

    // Отрисовка треугольников
    render_faces(render_ctx, overlay_ctx);

    // Отрисовка рёбер
    render_edges(render_ctx, overlay_ctx);

    // Отрисовка точек
    render_points(render_ctx, overlay_ctx);

    // Отрисовка bbox
    render_bbox(render_ctx, overlay_ctx);

    // Отрисовка координатной сетки
    render_grid(render_ctx, overlay_ctx);

    // Отрисовка координатной сетки
    render_navigation_center(render_ctx, overlay_ctx);

}

#endif    // DISPLAY_DRAW_C
