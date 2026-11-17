#ifndef OVERLAYS_C
#define OVERLAYS_C

#include "render.h"

// Номера буферов вершин для координатной сетки
uint32_t BUFFER_AXIS_X      = 0;
uint32_t BUFFER_AXIS_Y      = 1;
uint32_t BUFFER_AXIS_Z      = 2;
uint32_t BUFFER_COORD_LINES = 3;
uint32_t BUFFER_COUNT       = 4;

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

void generate_bbox(Application * app) {

    // размер вершин bbox (8 вершин параллелепипеда)
    size_t bbox_verts_size = sizeof(float) * 3 * 8;

    for (uint32_t i=0; i<app->overlay_ctx.bbox_children_count; i++) {

        // Выделяем память для вершин bbox
        float* bbox_vertices = malloc(bbox_verts_size);
    
        // Генерация координат вершин bbox

        OgfFile* ogf_file = (OgfFile*) app->file;

        // Нижняя часть коробки

        // Левые вершины

        bbox_vertices[ 0] = ogf_file->children[i].bbox_min[0];
        bbox_vertices[ 1] = ogf_file->children[i].bbox_min[1];
        bbox_vertices[ 2] = ogf_file->children[i].bbox_min[2];

        bbox_vertices[ 3] = ogf_file->children[i].bbox_min[0];
        bbox_vertices[ 4] = ogf_file->children[i].bbox_min[1];
        bbox_vertices[ 5] = ogf_file->children[i].bbox_max[2];

        // Правые вершины

        bbox_vertices[ 6] = ogf_file->children[i].bbox_max[0];
        bbox_vertices[ 7] = ogf_file->children[i].bbox_min[1];
        bbox_vertices[ 8] = ogf_file->children[i].bbox_min[2];

        bbox_vertices[ 9] = ogf_file->children[i].bbox_max[0];
        bbox_vertices[10] = ogf_file->children[i].bbox_min[1];
        bbox_vertices[11] = ogf_file->children[i].bbox_max[2];

        // Верхняя часть корoбки

        bbox_vertices[12] = ogf_file->children[i].bbox_min[0];
        bbox_vertices[13] = ogf_file->children[i].bbox_max[1];
        bbox_vertices[14] = ogf_file->children[i].bbox_min[2];

        bbox_vertices[15] = ogf_file->children[i].bbox_min[0];
        bbox_vertices[16] = ogf_file->children[i].bbox_max[1];
        bbox_vertices[17] = ogf_file->children[i].bbox_max[2];

        bbox_vertices[18] = ogf_file->children[i].bbox_max[0];
        bbox_vertices[19] = ogf_file->children[i].bbox_max[1];
        bbox_vertices[20] = ogf_file->children[i].bbox_min[2];

        bbox_vertices[21] = ogf_file->children[i].bbox_max[0];
        bbox_vertices[22] = ogf_file->children[i].bbox_max[1];
        bbox_vertices[23] = ogf_file->children[i].bbox_max[2];

        // Создать вершинный буфер для bbox
        glGenVertexArrays(1, &app->overlay_ctx.bbox_vao[i]);
        glBindVertexArray(app->overlay_ctx.bbox_vao[i]);

        glGenBuffers(1, &app->overlay_ctx.bbox_vertex_buffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, app->overlay_ctx.bbox_vertex_buffers[i]);

        glBufferData(GL_ARRAY_BUFFER, bbox_verts_size, bbox_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

        // Настройка атрибута позиции
        glEnableVertexAttribArray(0);

        // Создать индексный буфер для bbox

        // 12 рёбер куба
        uint32_t bbox_indices_count = 12 * 2;
        size_t bbox_indices_size = sizeof(uint16_t) * (size_t)bbox_indices_count;

        uint16_t* bbox_indices = malloc(bbox_indices_size);

        // генерировать индексы рёбер bbox

        // нижние рёбра
        bbox_indices[ 0] = 0;
        bbox_indices[ 1] = 1;

        bbox_indices[ 2] = 2;
        bbox_indices[ 3] = 3;

        bbox_indices[ 4] = 0;
        bbox_indices[ 5] = 2;

        bbox_indices[ 6] = 1;
        bbox_indices[ 7] = 3;

        // верхние рёбра
        bbox_indices[ 8] = 4;
        bbox_indices[ 9] = 5;

        bbox_indices[10] = 6;
        bbox_indices[11] = 7;

        bbox_indices[12] = 4;
        bbox_indices[13] = 6;

        bbox_indices[14] = 5;
        bbox_indices[15] = 7;

        // боковые рёбра
        bbox_indices[16] = 0;
        bbox_indices[17] = 4;

        bbox_indices[18] = 1;
        bbox_indices[19] = 5;

        bbox_indices[20] = 2;
        bbox_indices[21] = 6;

        bbox_indices[22] = 3;
        bbox_indices[23] = 7;

        glGenBuffers(1, &app->overlay_ctx.bbox_index_buffers[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->overlay_ctx.bbox_index_buffers[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bbox_indices_size, bbox_indices, GL_STATIC_DRAW);

        app->overlay_ctx.bbox_indices_count[i] = bbox_indices_count;

    }

}

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
