#ifndef RENDER_H
#define RENDER_H

typedef struct RenderContext {

    // Цвет очистки окна
    float clear_color[3];

    // Цвет мешей по-умолчанию
    float meshes_color_def[3];

    // Цвет рёбер меша
    float edges_color[3];

    // Цвет точек меша
    float points_color[3];

    // Цвет координатной сетки
    float grid_color[3];

    // Цвет осей
    float x_axis_color[3];
    float y_axis_color[3];
    float z_axis_color[3];

    // Цвета мешей
    float * meshes_color;

    // Центр модели
    float model_center[3];

    // Отсекать задние грани
    bool cull_face;

    // Отсекать задние рёбра
    bool cull_edge;

    // Вертикальная синхронизация
    bool vsync;

    // режим без освещения
    bool shadeless;

    // отображать текстуры
    bool use_textures;

    GLuint*     vao;
    GLuint*     vertex_buffers;
    GLuint*     index_buffers;
    GLuint*     textures;
    GLuint      ShaderProgram;
    GLuint      ShaderProgramNavCenter;
    GLint       useShadelessLoc;
    GLint       useTextureLoc;
    GLint       solidColorLoc;
    GLint       lightDirLoc;
    GLint       NavSolidColorLoc;
    GLint       uniform_pan;
    GLint       uniform_zoom;
    uint32_t*   indices_count;
    uint32_t*   vertices_count;
    uint32_t    children_count;

} RenderContext;

#endif    // RENDER_H
