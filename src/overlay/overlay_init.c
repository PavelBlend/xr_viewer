#pragma once

#include "..\overlay\overlay_grid.c"
#include "..\overlay\overlay_nav_center.c"

// Инициализировать буферы наложений
void init_overlay_buffs(Application * app) {

    app->overlay_ctx.vao            = NULL;
    app->overlay_ctx.vertex_buffers = NULL;
    app->overlay_ctx.index_buffers  = NULL;
    app->overlay_ctx.indices_count  = NULL;

    app->overlay_ctx.vao_nav            = NULL;
    app->overlay_ctx.nav_vertex_buffers = NULL;
    app->overlay_ctx.nav_index_buffers  = NULL;
    app->overlay_ctx.nav_indices_count  = NULL;

}

// Инициализировать наложения
void init_overlay(Application * app) {

    // Установить буферы на нулевой указатель
    init_overlay_buffs(app);

    // Генерация координатной сетки
    generate_grid(app);

    // Генерация центра навигации
    generate_nav_center(app);

}
