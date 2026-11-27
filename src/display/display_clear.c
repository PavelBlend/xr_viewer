#pragma once

void display_clear(RenderContext * render_ctx) {

    // Установить цвет очистки экрана
    glClearColor(render_ctx->clear_color[0], render_ctx->clear_color[1], render_ctx->clear_color[2], 1.0);

    // Очистка экрана
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
