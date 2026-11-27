#pragma once

typedef struct OverlayContext {

    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    uint32_t*   indices_count;

    GLuint*     bbox_vao;
    GLuint*     bbox_vertex_buffers;
    GLuint*     bbox_index_buffers;
    uint32_t*   bbox_indices_count;
    uint32_t    bbox_children_count;

    GLuint*     vao_nav;
    GLuint*     nav_vertex_buffers;
    GLuint*     nav_index_buffers;
    GLuint*     nav_indices_count;

    int32_t     grid_steps;     // Количество линий в каждую сторону
    float       grid_size;      // Половина размера сетки

    float       point_size;     // размер точек
    float       edge_width;     // Толщина линий сетки

    bool draw_faces;
    bool draw_edges;
    bool draw_points;
    bool draw_bbox;
    bool draw_grid;
    bool draw_axis_x;
    bool draw_axis_y;
    bool draw_axis_z;
    bool draw_navigation_center;

} OverlayContext;
