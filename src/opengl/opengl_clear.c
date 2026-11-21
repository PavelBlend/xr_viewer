#ifndef OPENGL_CLEAR
#define OPENGL_CLEAR

#include "..\tex_io.c"

void clear_gl_data(RenderContext * render_ctx) {

    // Очистка кэша текстур
    tex_cache_free();

    // Очистка ресурсов OpenGL
    for (uint32_t i = 0; i < render_ctx->children_count; i++) {
        glDeleteVertexArrays(1, &render_ctx->vao[i]);
        glDeleteBuffers(1, &render_ctx->vertex_buffers[i]);
        glDeleteBuffers(1, &render_ctx->index_buffers[i]);
        if (render_ctx->textures[i]) {
            glDeleteTextures(1, &render_ctx->textures[i]);
        }
    }

    // Освобождение выделенной памяти

    if (render_ctx->vao != NULL) {
        free(render_ctx->vao);
    }

    if (render_ctx->vertex_buffers != NULL) {
        free(render_ctx->vertex_buffers);
    }

    if (render_ctx->index_buffers != NULL) {
        free(render_ctx->index_buffers);
    }

    if (render_ctx->indices_count != NULL) {
        free(render_ctx->indices_count);
    }

    if (render_ctx->textures != NULL) {
        free(render_ctx->textures);
    }

    // Удаление шейдерной программы
    glDeleteProgram(render_ctx->ShaderProgram);

}

#endif    // OPENGL_CLEAR
