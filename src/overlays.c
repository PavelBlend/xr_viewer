#ifndef OVERLAYS_C
#define OVERLAYS_C

#include "render.h"

const float gridSize = 5.0f;    // Половина размера сетки
const int gridSteps = 10;        // Количество линий в каждую сторону

void render_navigation_center(RenderContext ctx) {

    glUniform3f(ctx.solidColorLoc, 1.0f, 1.0f, 1.0f);

    static GLuint axis_vao = 0, axis_vbo = 0;
    static int axis_initialized = 0;

    if (!axis_initialized) {
        glGenVertexArrays(1, &axis_vao);
        glGenBuffers(1, &axis_vbo);
        axis_initialized = 1;
    }

    // 6 вершин: по две на каждую ось (±0.1 по оси)
    float axis_scale = 0.05f; // размер креста в мировых единицах
    float lines[18] = {
        pan_x - axis_scale, pan_y, pan_z,   pan_x + axis_scale, pan_y, pan_z,   // X
        pan_x, pan_y - axis_scale, pan_z,   pan_x, pan_y + axis_scale, pan_z,   // Y
        pan_x, pan_y, pan_z - axis_scale,   pan_x, pan_y, pan_z + axis_scale    // Z
    };

    glBindVertexArray(axis_vao);
    glBindBuffer(GL_ARRAY_BUFFER, axis_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0f);
}

void render_grid(RenderContext ctx) {

    glUniform3f(ctx.solidColorLoc, 0.25f, 0.25f, 0.25f);

    glLineWidth(0.5f);

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

    glUniform1i(ctx.useTextureLoc, 0);  // Выключаем текстуру
    glBindTexture(GL_TEXTURE_2D, 0);

    render_grid(ctx);
    render_axis(ctx);
    render_navigation_center(ctx);

}

#endif    // OVERLAYS_C
