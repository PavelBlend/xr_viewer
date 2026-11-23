#ifndef REND_INIT_C
#define REND_INIT_C

#include "rend_init_ogf.c"

// Установить нулевые указатели на буферы рендера
void set_null_buffers(Application * app) {
    app->render_ctx.vao             = NULL;
    app->render_ctx.vertex_buffers  = NULL;
    app->render_ctx.index_buffers   = NULL;
    app->render_ctx.indices_count   = NULL;
    app->render_ctx.vertices_count  = NULL;
    app->render_ctx.textures        = NULL;
    app->render_ctx.children_count = 0;
}

// Выделить память для буферов рендера
void malloc_render_buffers(Application * app) {
    OgfFile* ogf_file = (OgfFile*) app->file;
    app->render_ctx.vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->render_ctx.vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->render_ctx.index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->render_ctx.indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
    app->render_ctx.vertices_count  = malloc(sizeof(uint32_t)   * ogf_file->children_count);
    app->render_ctx.textures        = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->render_ctx.children_count  = ogf_file->children_count;
}

// Выделить память для буферов наложений
void malloc_overlay_buffers(Application * app) {

    OgfFile* ogf_file = (OgfFile*) app->file;

    // bbox
    app->overlay_ctx.bbox_vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->overlay_ctx.bbox_vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->overlay_ctx.bbox_index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
    app->overlay_ctx.bbox_indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
    app->overlay_ctx.bbox_children_count  = ogf_file->children_count;

}

// Выделить память для буферов
void malloc_buffers(Application * app) {
    malloc_render_buffers(app);
    malloc_overlay_buffers(app);
}

void create_buffers(Application * app) {

    // Выделить память для буферов
    malloc_buffers(app);

    // Сгенерировать bbox геометрию
    generate_bbox(app);

    // Вычислить центр модели
    calc_model_center(app);

    // Сгенерировать цвета для мешей
    gen_gray_meshes_color(app);

    // OGF файл
    OgfFile* ogf_file = (OgfFile*) app->file;

    // Создать буферы для рендера
    for (uint32_t child_id = 0; child_id < ogf_file->children_count; child_id++) {
        render_init_ogf(app, child_id);
    }

}

void set_model_center(Application * app) {
    // центр модели
    app->render_ctx.model_center[0] = 0.0f;
    app->render_ctx.model_center[1] = 0.0f;
    app->render_ctx.model_center[2] = 0.0f;
}

// Инициализировать ресурсы рендера
void init_render_resources(Application * app) {

    // Установить центр модели
    set_model_center(app);

    if (app->file != NULL) {
        create_buffers(app);

    } else {
        set_null_buffers(app);
    }

}

#endif    // REND_INIT_C
