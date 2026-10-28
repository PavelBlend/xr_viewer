#ifndef RENDER_H
#define RENDER_H

#include "GL\glew.h"

bool vsync = true;
bool running = true;

typedef struct RenderContext {

    // Цвет очистки окна
    float clear_color[3];

    // Цвет рёбер меша
    float edges_color[3];

    // Цвет координатной сетки
    float grid_color[3];

    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    GLuint*     textures;
    GLuint      ShaderProgram;
    GLint       useShadelessLoc;
    GLint       useTextureLoc;
    GLint       solidColorLoc;
    GLint       lightDirLoc;
    uint32_t*   indices_count;
    uint32_t    children_count;

} RenderContext;

typedef struct OverlayContext {

    uint32_t    grid_steps;    // Количество линий в каждую сторону
    float       grid_size;    // Половина размера сетки
    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    uint32_t*   indices_count;

} OverlayContext;

OverlayContext overlay_ctx;

#endif    // RENDER_H
