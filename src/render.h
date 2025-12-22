#ifndef RENDER_H
#define RENDER_H

#include "GL\glew.h"
#include "GL\freeglut.h"

typedef struct RenderContext {
    GLuint*     vao;
    GLuint*     textures;
    GLuint      ShaderProgram;
    GLint       useTextureLoc;
    GLint       solidColorLoc;
    uint32_t*   indices_count;
    uint32_t    children_count;
} RenderContext;

#endif    // RENDER_H
