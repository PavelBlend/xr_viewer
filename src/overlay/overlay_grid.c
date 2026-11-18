#ifndef OVERLAYS_C
#define OVERLAYS_C

#include "..\render.h"

#include "overlay_const.h"

#include "overlay_bbox.c"
#include "overlay_coord_lines.c"
#include "overlay_axis.c"

// Сгенерировать геометрию координатной сетки
void generate_grid(Application * app) {

    OverlayContext* overlay_ctx = &app->overlay_ctx;

    // Настроить overlay контекст

    if (overlay_ctx->vao != NULL) {
        free(overlay_ctx->vao);
    }
    overlay_ctx->vao = malloc(sizeof(GLuint) * BUFFER_COUNT);

    if (overlay_ctx->vertex_buffers != NULL) {
        free(overlay_ctx->vertex_buffers);
    }
    overlay_ctx->vertex_buffers = malloc(sizeof(GLuint) * BUFFER_COUNT);

    if (overlay_ctx->index_buffers != NULL) {
        free(overlay_ctx->index_buffers);
    }
    overlay_ctx->index_buffers = malloc(sizeof(GLuint) * BUFFER_COUNT);

    if (overlay_ctx->indices_count != NULL) {
        free(overlay_ctx->indices_count);
    }
    overlay_ctx->indices_count = malloc(sizeof(uint32_t) * BUFFER_COUNT);

    // Сгенерировать геометрию координатной сетки
    generate_coord_lines(overlay_ctx);

    // Сгенерировать геометрию координатных осей
    generate_axis(overlay_ctx, BUFFER_AXIS_X);
    generate_axis(overlay_ctx, BUFFER_AXIS_Y);
    generate_axis(overlay_ctx, BUFFER_AXIS_Z);

}

#endif    // OVERLAYS_C
