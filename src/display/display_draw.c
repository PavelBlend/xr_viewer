#ifndef DISPLAY_DRAW_C
#define DISPLAY_DRAW_C

#include "display_clear.c"
#include "display_vsync.c"

#include "..\light\light_utils.c"

void display_draw(GLFWwindow* window, Application* app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    // установить вертикальную синхронизацию
    set_vsync(render_ctx);

    // Очистить экран
    display_clear(render_ctx);

    // Использовать шейдерную программу
    glUseProgram(render_ctx->ShaderProgram);

    // Установить матрицы для шейдера
    set_shader_matrices(window, render_ctx->ShaderProgram);

    // Добавить источник света
    add_light(window, render_ctx);

    // отрисовка треугольников
    render_faces(render_ctx, overlay_ctx);

    // отрисовка рёбер
    render_edges(render_ctx, overlay_ctx);

    // отрисовка bbox
    render_bbox(render_ctx, overlay_ctx);

    // отрисовка координатной сетки
    render_grid(render_ctx, overlay_ctx);

}

#endif    // DISPLAY_DRAW_C
