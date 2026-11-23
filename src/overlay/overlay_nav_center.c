#ifndef OVERLAY_NAV_CENTER_C
#define OVERLAY_NAV_CENTER_C

// сгенерировать перекрестие для центра навигации (3 ребра)
void generate_nav_center(Application * app) {

    // navigation center
    app->overlay_ctx.vao_nav             = malloc(sizeof(GLuint));
    app->overlay_ctx.nav_vertex_buffers  = malloc(sizeof(GLuint));
    app->overlay_ctx.nav_index_buffers   = malloc(sizeof(GLuint));
    app->overlay_ctx.nav_indices_count   = malloc(sizeof(uint32_t));

    OverlayContext* overlay_ctx = &app->overlay_ctx;

    // Количество вершин для ребра
    uint32_t edge_verts_count = 2;

    // Количество рёбер
    uint32_t edges_count = 3;

    // Количество всех вершин
    uint32_t verts_count = edge_verts_count * edges_count;

    // Размер одной вершины
    size_t vert_size = sizeof(float) * 3;

    // Размер всех вершин центра
    size_t verts_size = vert_size * (size_t)verts_count;

    // Создать вершины
    float* vertices = malloc(verts_size);

    // Указать координаты

    vertices[ 0]     = -1.0f;
    vertices[ 1]     =  0.0f;
    vertices[ 2]     =  0.0f;

    vertices[ 3]     =  1.0f;
    vertices[ 4]     =  0.0f;
    vertices[ 5]     =  0.0f;

    vertices[ 6]     =  0.0f;
    vertices[ 7]     = -1.0f;
    vertices[ 8]     =  0.0f;

    vertices[ 9]     =  0.0f;
    vertices[10]     =  1.0f;
    vertices[11]     =  0.0f;

    vertices[12]     =  0.0f;
    vertices[13]     =  0.0f;
    vertices[14]     = -1.0f;

    vertices[15]     =  0.0f;
    vertices[16]     =  0.0f;
    vertices[17]     =  1.0f;

    // Создать вершинный буфер для двух вершин
    glGenVertexArrays(1, &overlay_ctx->vao_nav[0]);
    glBindVertexArray(overlay_ctx->vao_nav[0]);

    glGenBuffers(1, &overlay_ctx->nav_vertex_buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, overlay_ctx->nav_vertex_buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, verts_size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    // Сгенерировать индексы
    uint16_t indices_count = edges_count * 2;
    size_t indices_size = sizeof(uint16_t) * (size_t)indices_count;
    uint16_t* indices = malloc(indices_size);

    for (uint16_t i=0; i<indices_count; i++) {
        indices[i] = i;
    }

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0);

    // Создать индексный буфер для оси
    glGenBuffers(1, &overlay_ctx->nav_index_buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_ctx->nav_index_buffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    overlay_ctx->nav_indices_count[0] = indices_count;

}

#endif    // OVERLAY_NAV_CENTER_C
