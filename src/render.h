#ifndef RENDER_H
#define RENDER_H

#include "GL\glew.h"

typedef struct RenderContext {

    // Цвет очистки окна
    float clear_color[3];

    // Цвет рёбер меша
    float edges_color[3];

    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    GLuint*     textures;
    GLuint      ShaderProgram;
    GLint       useTextureLoc;
    GLint       solidColorLoc;
    uint32_t*   indices_count;
    uint32_t    children_count;

} RenderContext;

#endif    // RENDER_H
