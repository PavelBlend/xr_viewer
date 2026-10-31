#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>

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

    // Цвет осей
    float x_axis_color[3];
    float y_axis_color[3];
    float z_axis_color[3];

    // Центр модели
    float model_center[3];

    // Отсекать задние грани
    bool cull_face;

    // Отсекать задние рёбра
    bool cull_edge;

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

// Создать контекст рендера
RenderContext ctx;

// Создать контекст наложений
OverlayContext overlay_ctx;

#endif    // RENDER_H
