#ifndef REND_FACES_C
#define REND_FACES_C

void render_faces(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    // рандомный цвет меша
    float r, g, b;

    // отрисовка треугольников
    if (overlay_ctx->draw_faces) {

        if (render_ctx->cull_face) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // сместить полигоны в буфере глубины, чтобы не было артефактов на wireframe
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        if (render_ctx->shadeless) {
            glUniform1i(render_ctx->useShadelessLoc, 1);
        } else {
            glUniform1i(render_ctx->useShadelessLoc, 0);
        }

        for (uint32_t child_id=0; child_id<render_ctx->children_count; child_id++) {

            if (render_ctx->textures[child_id] && render_ctx->use_textures) {
                glUniform1i(render_ctx->useTextureLoc, 1);  // Включаем текстуру
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, render_ctx->textures[child_id]);
            } else {
                glUniform1i(render_ctx->useTextureLoc, 0);
                random_color_from_id(child_id, &r, &g, &b);
                glUniform3f(render_ctx->solidColorLoc, r, g, b);
            }

            glBindVertexArray(render_ctx->vao[child_id]);
            glDrawElements(GL_TRIANGLES, render_ctx->indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }

        // выключить смещение полигонов в Z буфере
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

}

#endif    // REND_FACES_C
