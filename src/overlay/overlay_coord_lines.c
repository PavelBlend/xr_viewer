#pragma once

// Сгенерировать основные линии координатной сетки, без линий в нулевых координатах
// Линии в нулевых координатах генерировать не нужно, так как вместо них будут глобальные оси
void generate_coord_lines(OverlayContext* overlay_ctx) {

    // Вычислить количество линий координатной сетки:
    // overlay_ctx->grid_steps - количество линий для половины сетки.
    // Всего два вида линий: параллельно оси X и параллельно оси Z, поэтому умножаем на 2.
    // Все линии располагаются в положительную и в отрицательную сторону оси, поэтому ещё умножаем на 2.
    uint32_t lines_count = overlay_ctx->grid_steps * 2 * 2;

    // Вычислить количество вершин:
    // Количество линий нужно умножить на 2, так как одна линия строится на друх вершинах.
    uint32_t grid_verts_count = lines_count * 2;

    // Вычислить размер вершин сетки:
    // Вершина - это вектор с тремя элементами X, Y, Z с типом float.
    size_t grid_verts_size = sizeof(float) * 3 * grid_verts_count;

    // Выделяем память для вершин сетки
    float* grid_vertices = malloc(grid_verts_size);

    uint32_t vert_index = 0;

    // Линии X
    for (int32_t i = -((int32_t)overlay_ctx->grid_steps); i <= (int32_t)overlay_ctx->grid_steps; i++) {
        if (i != 0) {    // пропускаем линии в нулевых координатах
            float pos = i * overlay_ctx->grid_size;

            // Две вершины линии
            grid_vertices[vert_index + 0] = -overlay_ctx->grid_size * overlay_ctx->grid_steps;
            grid_vertices[vert_index + 1] = 0.0f;
            grid_vertices[vert_index + 2] = pos;

            grid_vertices[vert_index + 3] = +overlay_ctx->grid_size * overlay_ctx->grid_steps;
            grid_vertices[vert_index + 4] = 0.0f;
            grid_vertices[vert_index + 5] = pos;

            vert_index += 6;
        }
    }

    // Линии Z
    for (int i = -((int32_t)overlay_ctx->grid_steps); i <= (int32_t)overlay_ctx->grid_steps; i++) {
        if (i != 0) {    // пропускаем линии в нулевых координатах
            float pos = i * overlay_ctx->grid_size;

            // Две вершины линии
            grid_vertices[vert_index + 0] = pos;
            grid_vertices[vert_index + 1] = 0.0f;
            grid_vertices[vert_index + 2] = -overlay_ctx->grid_size * overlay_ctx->grid_steps;

            grid_vertices[vert_index + 3] = pos;
            grid_vertices[vert_index + 4] = 0.0f;
            grid_vertices[vert_index + 5] = +overlay_ctx->grid_size * overlay_ctx->grid_steps;

            vert_index += 6;
        }
    }

    // Создать вершинный буфер для координатной сетки
    glGenVertexArrays(1, &overlay_ctx->vao[BUFFER_COORD_LINES]);
    glBindVertexArray(overlay_ctx->vao[BUFFER_COORD_LINES]);

    glGenBuffers(1, &overlay_ctx->vertex_buffers[BUFFER_COORD_LINES]);
    glBindBuffer(GL_ARRAY_BUFFER, overlay_ctx->vertex_buffers[BUFFER_COORD_LINES]);

    glBufferData(GL_ARRAY_BUFFER, grid_verts_size, grid_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0);

    // Создать индексный буфер для координатной сетки

    uint32_t grid_indices_count = lines_count * 2;
    size_t grid_indices_size = sizeof(uint16_t) * (size_t)grid_indices_count;

    uint16_t* grid_indices = malloc(grid_indices_size);

    // генерировать индексы координатной сетки
    uint16_t current_index = 0;
    for (uint16_t i=0; i<grid_indices_count; i++) {
        grid_indices[i] = current_index;
        current_index++;
    }

    glGenBuffers(1, &overlay_ctx->index_buffers[BUFFER_COORD_LINES]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_ctx->index_buffers[BUFFER_COORD_LINES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid_indices_size, grid_indices, GL_STATIC_DRAW);

    overlay_ctx->indices_count[BUFFER_COORD_LINES] = grid_indices_count;

}
