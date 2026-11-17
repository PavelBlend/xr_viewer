#ifndef REND_INIT_OGF_C
#define REND_INIT_OGF_C

#define POS_ATTR    0
#define NORM_ATTR   1
#define UV_ATTR     2

// Вычислить центр модели
void calc_model_center(Application * app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // OGF файл
    OgfFile* ogf_file = (OgfFile*) app->file;

    // Центр
    render_ctx->model_center[0] = ogf_file->bbox_min[0] + (ogf_file->bbox_max[0] - ogf_file->bbox_min[0]) / 2;
    render_ctx->model_center[1] = ogf_file->bbox_min[1] + (ogf_file->bbox_max[1] - ogf_file->bbox_min[1]) / 2;
    render_ctx->model_center[2] = ogf_file->bbox_min[2] + (ogf_file->bbox_max[2] - ogf_file->bbox_min[2]) / 2;

}

// Загрузить текстуру
void load_ogf_texture(Application * app, uint32_t child_id, OgfChild * ogf_child) {
    app->render_ctx.textures[child_id] = load_dds_texture(ogf_child->texture_path);
}

// Создать индексный буфер
void create_indices_buffer(Application * app, uint32_t child_id, OgfChild * ogf_child) {

    // Сгенерировать буфер
    glGenBuffers(1, &app->render_ctx.index_buffers[child_id]);

    // Привязать буфер к цели
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->render_ctx.index_buffers[child_id]);

    // Заполнить буфер данными
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * ogf_child->indices->count, ogf_child->indices->indices, GL_STATIC_DRAW);

    // Сохранить количество индексов
    app->render_ctx.indices_count[child_id] = ogf_child->indices->count;

}

// Создать Vertex Array Object
void create_vao(Application * app, uint32_t child_id) {

    // Сгенерировать объект
    glGenVertexArrays(1, &app->render_ctx.vao[child_id]);

    // Привязать объект
    glBindVertexArray(app->render_ctx.vao[child_id]);

}

// Включить использование атрибутов вершин
void enable_vertex_attrs(void) {
    glEnableVertexAttribArray(POS_ATTR);    // Позиция
    glEnableVertexAttribArray(NORM_ATTR);   // Нормаль
    glEnableVertexAttribArray(UV_ATTR);     // Текстурные координаты
}

// Обновить статистику
void update_stats(Application * app, OgfChild * ogf_child) {
    app->stats.total_vertices   += ogf_child->verts->count;
    app->stats.total_indices    += ogf_child->indices->count;
}

// Сгенерировать вершинный буфер
void gen_vertex_buffer(Application * app, uint32_t child_id) {

    // Сгенерировать буфер
    glGenBuffers(1, &app->render_ctx.vertex_buffers[child_id]);

    // Привязать объект
    glBindBuffer(GL_ARRAY_BUFFER, app->render_ctx.vertex_buffers[child_id]);

}

void set_vertex_buffer_data(OgfChild * ogf_child) {

    // Размер буфера вершин
    size_t verts_size;

    if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
        verts_size = ogf_child->verts->count * 60;
        glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_1_link, GL_STATIC_DRAW);
        glVertexAttribPointer(POS_ATTR,  3, GL_FLOAT, GL_FALSE, 60, (void*)  0);    // position
        glVertexAttribPointer(NORM_ATTR, 3, GL_FLOAT, GL_FALSE, 60, (void*) 12);    // normal
        glVertexAttribPointer(UV_ATTR,   2, GL_FLOAT, GL_FALSE, 60, (void*) 48);    // uv attribute

    } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
        verts_size = ogf_child->verts->count * 64;
        glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_2_link, GL_STATIC_DRAW);
        glVertexAttribPointer(POS_ATTR,  3, GL_FLOAT, GL_FALSE, 64, (void*)  4);    // position
        glVertexAttribPointer(NORM_ATTR, 3, GL_FLOAT, GL_FALSE, 64, (void*) 16);    // normal
        glVertexAttribPointer(UV_ATTR,   2, GL_FLOAT, GL_FALSE, 64, (void*) 56);    // uv attribute

    } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
        verts_size = ogf_child->verts->count * 70;
        glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_3_link, GL_STATIC_DRAW);
        glVertexAttribPointer(POS_ATTR,  3, GL_FLOAT, GL_FALSE, 70, (void*)  6);    // position
        glVertexAttribPointer(NORM_ATTR, 3, GL_FLOAT, GL_FALSE, 70, (void*) 18);    // normal
        glVertexAttribPointer(UV_ATTR,   2, GL_FLOAT, GL_FALSE, 70, (void*) 62);    // uv attribute

    } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
        verts_size = ogf_child->verts->count * 76;
        glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_4_link, GL_STATIC_DRAW);
        glVertexAttribPointer(POS_ATTR,  3, GL_FLOAT, GL_FALSE, 76, (void*)  8);    // position
        glVertexAttribPointer(NORM_ATTR, 3, GL_FLOAT, GL_FALSE, 76, (void*) 20);    // normal
        glVertexAttribPointer(UV_ATTR,   2, GL_FLOAT, GL_FALSE, 76, (void*) 68);    // uv attribute

    } else if (ogf_child->verts->format == FVF_OGF) {
        verts_size = ogf_child->verts->count * 32;
        glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_static, GL_STATIC_DRAW);
        glVertexAttribPointer(POS_ATTR,  3, GL_FLOAT, GL_FALSE, 32, (void*)  0);    // position
        glVertexAttribPointer(NORM_ATTR, 3, GL_FLOAT, GL_FALSE, 32, (void*) 12);    // normal
        glVertexAttribPointer(UV_ATTR,   2, GL_FLOAT, GL_FALSE, 32, (void*) 24);    // uv attribute

    } else {
        printf("unsupported rendering for vertex format: 0x%x\n", ogf_child->verts->format);
        exit(EXIT_FAILURE);
    }

}

void create_vertex_buffer(Application * app, uint32_t child_id, OgfChild * ogf_child) {

    // Сгенерировать вершинный буфер
    gen_vertex_buffer(app, child_id);

    // Заполнить буфер данными
    set_vertex_buffer_data(ogf_child);

    // Включить использование атрибутов вершин
    enable_vertex_attrs();

}

void render_init_ogf(Application * app, uint32_t child_id) {

    // OGF файл
    OgfFile* ogf_file = (OgfFile*) app->file;

    // Child-визуал
    OgfChild * ogf_child = &ogf_file->children[child_id];

    // Создать VAO
    create_vao(app, child_id);

    // Создать вершинный буфер
    create_vertex_buffer    (app, child_id, ogf_child);

    // Создать индексный буфер
    create_indices_buffer   (app, child_id, ogf_child);

    // Загрузить текстуру
    load_ogf_texture        (app, child_id, ogf_child);

    // Обновить статистику
    update_stats            (app, ogf_child);

}

#endif    // REND_INIT_OGF_C
