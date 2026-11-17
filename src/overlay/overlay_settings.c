#ifndef OVERLAY_SETTINGS_C
#define OVERLAY_SETTINGS_C

void set_overlay_settings(Application * app) {

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    // Количество клеток сетки в одну сторону (положительную/отрицательную).
    // Всего клеток будет в два раза больше.
    overlay_ctx->grid_steps     = 10;

    // Размер клетки
    overlay_ctx->grid_size      = 1.0f;

    // Отображать полигоны
    overlay_ctx->draw_faces     = true;

    // Отображать рёбра
    overlay_ctx->draw_edges     = false;

    // Отображать ограничивающие коробки
    overlay_ctx->draw_bbox      = false;

    // Отображать координатную сетку
    overlay_ctx->draw_grid      = true;

    // Отображать оси X/Y/Z
    overlay_ctx->draw_axis_x    = true;
    overlay_ctx->draw_axis_y    = true;
    overlay_ctx->draw_axis_z    = true;

}

#endif    // OVERLAY_SETTINGS_C
