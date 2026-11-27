#pragma once

void render_bbox(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    // отрисовка bbox
    if (overlay_ctx->draw_bbox) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        for (uint32_t child_id=0; child_id<overlay_ctx->bbox_children_count; child_id++) {

            glUniform1i(render_ctx->useTextureLoc, 0);
            glUniform1i(render_ctx->useShadelessLoc, 1);

            // Цвет линий bbox
            glUniform3f(
                render_ctx->solidColorLoc,
                render_ctx->meshes_color[child_id * 3],
                render_ctx->meshes_color[child_id * 3 + 1],
                render_ctx->meshes_color[child_id * 3 + 2]
            );

            glBindVertexArray(overlay_ctx->bbox_vao[child_id]);
            glDrawElements(GL_LINES, overlay_ctx->bbox_indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }
    }
}
