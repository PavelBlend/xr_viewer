#ifndef REND_BBOX_C
#define REND_BBOX_C

void render_bbox(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    // цвет линий bbox
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    // отрисовка bbox
    if (overlay_ctx->draw_bbox) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        for (uint32_t child_id=0; child_id<overlay_ctx->bbox_children_count; child_id++) {

            glUniform1i(render_ctx->useTextureLoc, 0);
            glUniform1i(render_ctx->useShadelessLoc, 1);
            random_color_from_id(child_id, &r, &g, &b);
            glUniform3f(render_ctx->solidColorLoc, r, g, b);

            glBindVertexArray(overlay_ctx->bbox_vao[child_id]);
            glDrawElements(GL_LINES, overlay_ctx->bbox_indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }
    }
}

#endif    // REND_BBOX_C
