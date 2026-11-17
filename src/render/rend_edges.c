#ifndef REND_EDGES_C
#define REND_EDGES_C

void render_edges(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    if (overlay_ctx->draw_edges) {

        if (render_ctx->cull_edge) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);

        for (uint32_t child_id=0; child_id<render_ctx->children_count; child_id++) {

            glUniform1i(render_ctx->useTextureLoc, 0);
            glUniform1i(render_ctx->useShadelessLoc, 1);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->edges_color[0], render_ctx->edges_color[1], render_ctx->edges_color[2]);

            glBindVertexArray(render_ctx->vao[child_id]);
            glDrawElements(GL_TRIANGLES, render_ctx->indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

}

#endif    // REND_EDGES_C
