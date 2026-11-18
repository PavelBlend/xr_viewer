#ifndef OVERLAY_INIT_C
#define OVERLAY_INIT_C

#include "..\overlay\overlay_grid.c"

// Инициализировать буферы наложений
void init_overlay_buffs(Application * app) {

    app->overlay_ctx.vao            = NULL;
    app->overlay_ctx.vertex_buffers = NULL;
    app->overlay_ctx.index_buffers  = NULL;
    app->overlay_ctx.indices_count  = NULL;

}

// Инициализировать наложения
void init_overlay(Application * app) {

    // Установить буферы на нулевой указатель
    init_overlay_buffs(app);

    // Генерация координатной сетки
    generate_grid(app);

}

#endif    // OVERLAY_INIT_C
