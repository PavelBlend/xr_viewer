#ifndef REND_INIT_C
#define REND_INIT_C

// Установить нулевые указатели на буферы рендера
void set_null_buffers(Application * app) {
    app->render_ctx.vao             = NULL;
    app->render_ctx.vertex_buffers  = NULL;
    app->render_ctx.index_buffers   = NULL;
    app->render_ctx.indices_count   = NULL;
    app->render_ctx.textures        = NULL;
    app->render_ctx.children_count = 0;
}

void init_render_resources(Application * app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    if (app->file != NULL) {
        OgfFile* ogf_file = (OgfFile*) app->file;

        render_ctx->vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        render_ctx->textures        = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->children_count  = ogf_file->children_count;

        overlay_ctx->bbox_vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        overlay_ctx->bbox_children_count  = ogf_file->children_count;

        // Сгенерировать bbox геометрию
        generate_bbox(app, overlay_ctx);

        // Вычислить центр модели
        render_ctx->model_center[0] = ogf_file->bbox_min[0] + (ogf_file->bbox_max[0] - ogf_file->bbox_min[0]) / 2;
        render_ctx->model_center[1] = ogf_file->bbox_min[1] + (ogf_file->bbox_max[1] - ogf_file->bbox_min[1]) / 2;
        render_ctx->model_center[2] = ogf_file->bbox_min[2] + (ogf_file->bbox_max[2] - ogf_file->bbox_min[2]) / 2;

        for (uint32_t child_id=0; child_id<ogf_file->children_count; child_id++) {

            // Создать VAO
            glGenVertexArrays(1, &render_ctx->vao[child_id]);
            glBindVertexArray(render_ctx->vao[child_id]);

            // Создать вершинный буфер
            glGenBuffers(1, &render_ctx->vertex_buffers[child_id]);
            glBindBuffer(GL_ARRAY_BUFFER, render_ctx->vertex_buffers[child_id]);

            size_t verts_size;

            OgfChild* ogf_child = &ogf_file->children[child_id];

            app->stats.total_vertices += ogf_child->verts->count;
            app->stats.total_indices += ogf_child->indices->count;

            if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
                verts_size = ogf_child->verts->count * 60;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_1_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, (void*) 0);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 60, (void*) 12);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 60, (void*) 48);    // uv attribute

            } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
                verts_size = ogf_child->verts->count * 64;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_2_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, (void*) 4);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (void*) 16);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 64, (void*) 56);    // uv attribute

            } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
                verts_size = ogf_child->verts->count * 70;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_3_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 70, (void*) 6);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 70, (void*) 18);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 70, (void*) 62);    // uv attribute

            } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
                verts_size = ogf_child->verts->count * 76;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_4_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 76, (void*) 8);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 76, (void*) 20);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 76, (void*) 68);    // uv attribute

            } else if (ogf_child->verts->format == FVF_OGF) {
                verts_size = ogf_child->verts->count * 32;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_static, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*) 0);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*) 12);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, (void*) 24);    // uv attribute

            } else {
                printf("unsupported rendering for vertex format: 0x%x\n", ogf_child->verts->format);
                exit(EXIT_FAILURE);
            }

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Создать буфер индексов
            glGenBuffers(1, &render_ctx->index_buffers[child_id]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_ctx->index_buffers[child_id]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * ogf_child->indices->count, ogf_child->indices->indices, GL_STATIC_DRAW);

            render_ctx->indices_count[child_id] = ogf_child->indices->count;

            // Загрузить текстуру
            render_ctx->textures[child_id] = load_dds_texture(ogf_child->texture_path);
            // printf("Load Texture: \"%s\"\n", ogf_child->texture_path);

        }

    } else {
        set_null_buffers(app);
    }

}

#endif    // REND_INIT_C
