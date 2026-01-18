#ifndef OVERLAYS_C
#define OVERLAYS_C

#include "render.h"

const float gridSize = 5.0f;    // Половина размера сетки
const int gridSteps = 10;        // Количество линий в каждую сторону
bool draw_edges = false;

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

    glUniform1i(ctx.useTextureLoc, 0);  // Выключаем текстуру
    glBindTexture(GL_TEXTURE_2D, 0);

    render_grid(ctx);
    render_axis(ctx);

}

#endif    // OVERLAYS_C
