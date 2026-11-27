#pragma once

void set_render_settings(Application * app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Установить параметр вертикальной синхронизации
    render_ctx->vsync = true;

    // Установить параметры шейдинга
    render_ctx->shadeless       = false;
    render_ctx->use_textures    = true;

    // Установить параметры отсечения
    render_ctx->cull_face = true;
    render_ctx->cull_edge = false;

    // Установить цвет для очистки экрана
    render_ctx->clear_color[0] = 0.5f;
    render_ctx->clear_color[1] = 0.5f;
    render_ctx->clear_color[2] = 0.5f;
    glClearColor(
        render_ctx->clear_color[0],
        render_ctx->clear_color[1],
        render_ctx->clear_color[2],
        1.0f
    );

    // Установить цвет меша
    render_ctx->meshes_color_def[0] = 0.75f;
    render_ctx->meshes_color_def[1] = 0.75f;
    render_ctx->meshes_color_def[2] = 0.75f;

    // Установить цвет рёбер меша
    render_ctx->edges_color[0] = 0.0f;
    render_ctx->edges_color[1] = 0.0f;
    render_ctx->edges_color[2] = 0.0f;

    // Установить цвет точек меша
    render_ctx->points_color[0] = 0.8f;
    render_ctx->points_color[1] = 0.8f;
    render_ctx->points_color[2] = 0.8f;

    // Установить цвет координатной сетки
    render_ctx->grid_color[0] = 0.45f;
    render_ctx->grid_color[1] = 0.45f;
    render_ctx->grid_color[2] = 0.45f;

    // Установить цвет оси X
    render_ctx->x_axis_color[0] = 0.5f;
    render_ctx->x_axis_color[1] = 0.0f;
    render_ctx->x_axis_color[2] = 0.0f;

    // Установить цвет оси Y
    render_ctx->y_axis_color[0] = 0.0f;
    render_ctx->y_axis_color[1] = 0.5f;
    render_ctx->y_axis_color[2] = 0.0f;

    // Установить цвет оси Z
    render_ctx->z_axis_color[0] = 0.0f;
    render_ctx->z_axis_color[1] = 0.0f;
    render_ctx->z_axis_color[2] = 0.5f;

    // Установить рандомные цвета мешей на NULL
    render_ctx->meshes_color = NULL;

}
