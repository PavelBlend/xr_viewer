#ifndef REND_NAV_CENTER_C
#define REND_NAV_CENTER_C

void render_navigation_center(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    // отрисовка центра навигации
    if (overlay_ctx->draw_navigation_center) {

        // Использовать шейдерную программу
        glUseProgram(render_ctx->ShaderProgramNavCenter);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);

        glUniform3f(render_ctx->uniform_pan,    pan_x, pan_y, -pan_z);
        glUniform1f(render_ctx->uniform_zoom,   zoom);
        glUniform3f(render_ctx->NavSolidColorLoc,  1.0f, 1.0f, 1.0f);

        glBindVertexArray(overlay_ctx->vao_nav[0]);
        glDrawElements(GL_LINES, overlay_ctx->nav_indices_count[0], GL_UNSIGNED_SHORT, 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

#endif    // REND_NAV_CENTER_C
