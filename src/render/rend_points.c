#pragma once

void render_points(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    if (overlay_ctx->draw_points) {

        // Отключаем отсечение — вершины не имеют нормалей/лицевой стороны
        glDisable(GL_CULL_FACE);

        // Режим точек
        glPointSize(overlay_ctx->point_size);

        for (uint32_t child_id = 0; child_id < render_ctx->children_count; child_id++) {

            // Отключить текстуры
            glUniform1i(render_ctx->useTextureLoc,   0);

            // Отключить освещение
            glUniform1i(render_ctx->useShadelessLoc, 1);

            // Установить цвет точек
            glUniform3f(
                render_ctx->solidColorLoc,
                render_ctx->points_color[0],
                render_ctx->points_color[1],
                render_ctx->points_color[2]
            );

            glBindVertexArray(render_ctx->vao[child_id]);

            // Рисование точек
            glDrawArrays(GL_POINTS, 0, render_ctx->vertices_count[child_id]);

        }

        glPointSize(1.0f);

    }

}
