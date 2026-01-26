#ifndef OVERLAYS_C
#define OVERLAYS_C

#include "render.h"

const float gridSize = 5.0f;    // Половина размера сетки
const int gridSteps = 10;        // Количество линий в каждую сторону
bool draw_edges = false;

// Сгенерировать геометрию координатной сетки
void generate_grid(OverlayContext* overlay_ctx) {

    // Настроить overlay контекст
    overlay_ctx->vao = malloc(sizeof(GLuint) * 4);
    overlay_ctx->vertex_buffers = malloc(sizeof(GLuint) * 4);
    overlay_ctx->grid_steps = 10;
    overlay_ctx->grid_size = 1.0f;

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
    glGenVertexArrays(1, &overlay_ctx->vao[0]);
    glBindVertexArray(overlay_ctx->vao[0]);

    glGenBuffers(1, &overlay_ctx->vertex_buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, overlay_ctx->vertex_buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, grid_verts_size, grid_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0);

    // Создать индексный буфер для координатной сетки
    overlay_ctx->index_buffers = malloc(sizeof(GLuint) * 4);
    overlay_ctx->indices_count = malloc(sizeof(uint32_t) * 4);

    uint32_t grid_indices_count = lines_count * 2;
    size_t grid_indices_size = sizeof(uint16_t) * (size_t)grid_indices_count;

    uint16_t* grid_indices = malloc(grid_indices_size);

    // генерировать индексы координатной сетки
    uint16_t current_index = 0;
    for (uint16_t i=0; i<grid_indices_count; i++) {
        grid_indices[i] = current_index;
        current_index++;
    }

    glGenBuffers(1, &overlay_ctx->index_buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_ctx->index_buffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid_indices_size, grid_indices, GL_STATIC_DRAW);

    overlay_ctx->indices_count[0] = grid_indices_count;

}

void render_grid(RenderContext ctx) {

    glUniform3f(ctx.solidColorLoc, 0.25f, 0.25f, 0.25f);

    glLineWidth(1.0f);

    glBegin(GL_LINES);

    // Оси X (параллельно Z)
    for (int i = -gridSteps; i <= gridSteps; i++) {
        float pos = i * (gridSize / gridSteps);
        if (pos != 0.0f) {
            glVertex3f(-gridSize, 0.0f, pos);
            glVertex3f( gridSize, 0.0f, pos);
        }
    }

    // Оси Z (параллельно X)
    for (int i = -gridSteps; i <= gridSteps; i++) {
        float pos = i * (gridSize / gridSteps);
        if (pos != 0.0f) {
            glVertex3f(pos, 0.0f, -gridSize);
            glVertex3f(pos, 0.0f,  gridSize);
        }
    }

    glEnd();

}

void render_axis(RenderContext ctx) {

    glLineWidth(1.5f);

    // Ось X
    glUniform3f(ctx.solidColorLoc, 0.5f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glVertex3f(-gridSize, 0.0f, 0.0f);
    glVertex3f(+gridSize, 0.0f, 0.0f);

    glEnd();

    // Ось Y
    glUniform3f(ctx.solidColorLoc, 0.0f, 0.5f, 0.0f);

    glBegin(GL_LINES);

    glVertex3f(0.0f, -gridSize, 0.0f);
    glVertex3f(0.0f, +gridSize, 0.0f);

    glEnd();

    // Ось Z
    glUniform3f(ctx.solidColorLoc, 0.0f, 0.0f, 0.5f);

    glBegin(GL_LINES);

    glVertex3f(0.0f, 0.0f,-gridSize);
    glVertex3f(0.0f, 0.0f,+gridSize);

    glEnd();

}

void render_overlays(RenderContext ctx) {

    if (0) {

        glUniform1i(ctx.useTextureLoc, 0);  // Выключаем текстуру
        glBindTexture(GL_TEXTURE_2D, 0);

        render_grid(ctx);
        render_axis(ctx);

    }

}

#endif    // OVERLAYS_C
