#ifndef REND_GRID_C
#define REND_GRID_C

void render_grid(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);

    for (uint32_t i=0; i<4; i++) {

        glUniform1i(render_ctx->useTextureLoc, 0);
        glUniform1i(render_ctx->useShadelessLoc, 1);

        if (i==0) {
            if (overlay_ctx->draw_axis_x) {    // цвет оси X
                glLineWidth(1.5f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->x_axis_color[0], render_ctx->x_axis_color[1], render_ctx->x_axis_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            } else if (overlay_ctx->draw_grid) {    // цвет сетки
                glLineWidth(1.0f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            }

        } else if (i==1 && overlay_ctx->draw_axis_y) {
            glLineWidth(1.5f);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->y_axis_color[0], render_ctx->y_axis_color[1], render_ctx->y_axis_color[2]);
            glBindVertexArray(overlay_ctx->vao[i]);
            glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);

        } else if (i==2) {
            if (overlay_ctx->draw_axis_z) {    // цвет оси Z
                glLineWidth(1.5f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->z_axis_color[0], render_ctx->z_axis_color[1], render_ctx->z_axis_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            } else if (overlay_ctx->draw_grid) {    // цвет сетки
                glLineWidth(1.0f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            }

        } else  if (i==3 && overlay_ctx->draw_grid) {
            glLineWidth(1.0f);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
            glBindVertexArray(overlay_ctx->vao[i]);
            glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
        }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

}

#endif    // REND_GRID_C
