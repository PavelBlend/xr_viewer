#ifndef OVERLAYS_H
#define OVERLAYS_H

#include <stdbool.h>

#include "GL\glew.h"

typedef struct OverlayContext {

    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    uint32_t*   indices_count;

    int32_t     grid_steps;    // Количество линий в каждую сторону
    float       grid_size;    // Половина размера сетки

    bool draw_faces;
    bool draw_edges;
    bool draw_grid;
    bool draw_axis_x;
    bool draw_axis_y;
    bool draw_axis_z;

} OverlayContext;

#endif    // OVERLAYS_H
