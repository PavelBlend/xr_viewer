#ifndef OVERLAY_COORD_AXIS_C
#define OVERLAY_COORD_AXIS_C

// Сгенерировать линии осей для координатной сетки
void generate_axis(OverlayContext* overlay_ctx, uint32_t axis) {

    // Количество вершин для оси
    uint32_t verts_count = 2;

    // Размер одной вершины
    size_t vert_size = sizeof(float) * 3;

    // Размер всех вершин оси
    size_t axis_verts_size = vert_size * (size_t)verts_count;

    // Создать вершины
    float* axis_vertices = malloc(axis_verts_size);;

    // Обнулить все координаты
    for (uint32_t i=0; i<6; i++) {
        axis_vertices[i] = 0.0f;
    }

    // Указать координаты оси
    axis_vertices[axis]     = -overlay_ctx->grid_size * overlay_ctx->grid_steps;
    axis_vertices[axis + 3] = +overlay_ctx->grid_size * overlay_ctx->grid_steps;

    // Создать вершинный буфер для двух вершин оси
    glGenVertexArrays(1, &overlay_ctx->vao[axis]);
    glBindVertexArray(overlay_ctx->vao[axis]);

    glGenBuffers(1, &overlay_ctx->vertex_buffers[axis]);
    glBindBuffer(GL_ARRAY_BUFFER, overlay_ctx->vertex_buffers[axis]);

    glBufferData(GL_ARRAY_BUFFER, axis_verts_size, axis_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    // Сгенерировать индексы
    size_t axis_indices_size = sizeof(uint16_t) * 2;
    uint16_t* axis_indices = malloc(axis_indices_size);

    axis_indices[0] = 0;
    axis_indices[1] = 1;

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0);

    // Создать индексный буфер для оси
    glGenBuffers(1, &overlay_ctx->index_buffers[axis]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_ctx->index_buffers[axis]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, axis_indices_size, axis_indices, GL_STATIC_DRAW);

    overlay_ctx->indices_count[axis] = 2;

}

#endif    // OVERLAY_COORD_AXIS_C
