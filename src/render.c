#ifndef RENDER_C
#define RENDER_C

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <windows.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "ogf_io.c"

#include "cimgui.h"
#include "cimgui_impl.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "cglm\cglm.h"
#include "FreeImage.h"

#include "io_fun.c"
#include "globals.c"
#include "tex_io.c"
#include "shaders.c"
#include "overlays.c"
#include "formating.c"
#include "color_utils.c"

#include "render.h"
#include "types.h"
#include "application.h"
#include "ogf_verts.h"

#include "ui\im_gui_init.c"
#include "ui\im_gui_deinit.c"
#include "ui\im_gui_draw.c"

#include "render\rend_faces.c"
#include "render\rend_edges.c"
#include "render\rend_bbox.c"
#include "render\rend_grid.c"
#include "render\rend_settings.c"
#include "overlay\overlay_settings.c"
#include "window\win_utils.c"
#include "display\display_draw.c"
#include "opengl\opengl_clear.c"
#include "opengl\opengl_settings.c"
#include "glfw\glfw_init.c"
#include "glfw\glfw_deinit.c"
#include "glfw\glfw_callbacks.c"
#include "glew\glew_init.c"

void render(Application * app) {

    // Инициализировать FreeImage
    init_free_image();

    // Инициализировать GLFW
    glfw_init();

    // Создать окно
    create_window(app);

    // Установить позицию и размер окна
    set_window_transforms(app);

    // Инициализировать GLEW
    glew_init();

    // Установить настройки рендера OpenGL
    set_opengl_render_settings();

    // Сгенерировать seed для генератора цвета
    init_color_seed();

    // Установить настройки рендера
    set_render_settings(app);

    // Установить настройки наложений
    set_overlay_settings(app);

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    overlay_ctx->vao            = NULL;
    overlay_ctx->vertex_buffers = NULL;
    overlay_ctx->index_buffers  = NULL;
    overlay_ctx->indices_count  = NULL;

    // Генерация координатной сетки
    generate_grid(overlay_ctx);

    // загрузить шейдеры
    render_ctx->ShaderProgram = compile_shaders(app->exe_dir);

    render_ctx->useShadelessLoc = glGetUniformLocation(render_ctx->ShaderProgram, "useShadeless");
    render_ctx->useTextureLoc   = glGetUniformLocation(render_ctx->ShaderProgram, "useTexture");
    render_ctx->solidColorLoc   = glGetUniformLocation(render_ctx->ShaderProgram, "solidColor");
    render_ctx->lightDirLoc     = glGetUniformLocation(render_ctx->ShaderProgram, "lightDir");

    // статистика
    uint32_t total_vertices = 0;
    uint32_t total_indices = 0;

    // центр модели
    render_ctx->model_center[0] = 0.0f;
    render_ctx->model_center[1] = 0.0f;
    render_ctx->model_center[2] = 0.0f;

    if (app->file != NULL) {
        OgfFile* ogf_file = (OgfFile*) app->file;

        render_ctx->vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        render_ctx->textures        = malloc(sizeof(GLuint)     * ogf_file->children_count);
        render_ctx->children_count  = ogf_file->children_count;

        overlay_ctx->bbox_vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
        overlay_ctx->bbox_indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        overlay_ctx->bbox_children_count  = ogf_file->children_count;

        // Сгенерировать bbox геометрию
        generate_bbox(app, overlay_ctx);

        // Вычислить центр модели
        render_ctx->model_center[0] = ogf_file->bbox_min[0] + (ogf_file->bbox_max[0] - ogf_file->bbox_min[0]) / 2;
        render_ctx->model_center[1] = ogf_file->bbox_min[1] + (ogf_file->bbox_max[1] - ogf_file->bbox_min[1]) / 2;
        render_ctx->model_center[2] = ogf_file->bbox_min[2] + (ogf_file->bbox_max[2] - ogf_file->bbox_min[2]) / 2;

        for (uint32_t child_id=0; child_id<ogf_file->children_count; child_id++) {

            // Создать VAO
            glGenVertexArrays(1, &render_ctx->vao[child_id]);
            glBindVertexArray(render_ctx->vao[child_id]);

            // Создать вершинный буфер
            glGenBuffers(1, &render_ctx->vertex_buffers[child_id]);
            glBindBuffer(GL_ARRAY_BUFFER, render_ctx->vertex_buffers[child_id]);

            size_t verts_size;

            OgfChild* ogf_child = &ogf_file->children[child_id];

            total_vertices += ogf_child->verts->count;
            total_indices += ogf_child->indices->count;

            if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
                verts_size = ogf_child->verts->count * 60;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_1_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, (void*) 0);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 60, (void*) 12);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 60, (void*) 48);    // uv attribute

            } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
                verts_size = ogf_child->verts->count * 64;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_2_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, (void*) 4);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 64, (void*) 16);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 64, (void*) 56);    // uv attribute

            } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
                verts_size = ogf_child->verts->count * 70;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_3_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 70, (void*) 6);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 70, (void*) 18);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 70, (void*) 62);    // uv attribute

            } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
                verts_size = ogf_child->verts->count * 76;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_4_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 76, (void*) 8);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 76, (void*) 20);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 76, (void*) 68);    // uv attribute

            } else if (ogf_child->verts->format == FVF_OGF) {
                verts_size = ogf_child->verts->count * 32;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_static, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*) 0);     // position
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*) 12);    // normal
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, (void*) 24);    // uv attribute

            } else {
                printf("unsupported rendering for vertex format: 0x%x\n", ogf_child->verts->format);
                exit(EXIT_FAILURE);
            }

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Создать буфер индексов
            glGenBuffers(1, &render_ctx->index_buffers[child_id]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_ctx->index_buffers[child_id]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * ogf_child->indices->count, ogf_child->indices->indices, GL_STATIC_DRAW);

            render_ctx->indices_count[child_id] = ogf_child->indices->count;

            // Загрузить текстуру
            render_ctx->textures[child_id] = load_dds_texture(ogf_child->texture_path);
            // printf("Load Texture: \"%s\"\n", ogf_child->texture_path);

        }

    } else {
        render_ctx->vao             = NULL;
        render_ctx->vertex_buffers  = NULL;
        render_ctx->index_buffers   = NULL;
        render_ctx->indices_count   = NULL;
        render_ctx->textures        = NULL;
        render_ctx->children_count = 0;
    }

    // Установка callback'ов GLFW
    set_glfw_callbacks(app);

    // Инициализация CImGUI
    c_im_gui_init(app->window, app->exe_dir);

    // Переменные для подсчёта FPS
    static double last_time = 0.0;
    static double fps = 0.0;

    // установить начальную позицию камеры
    pan_x = render_ctx->model_center[0];
    pan_y = render_ctx->model_center[1];
    pan_z = -render_ctx->model_center[2];

    // Главный цикл
    while (!glfwWindowShouldClose(app->window)) {

        if (app->running) {

            // Обработка событий
            glfwPollEvents();

            // рендер сцены
            display_draw(app->window, app);

            // рендер интерфейса
            draw_ui(app, &fps, &last_time, total_vertices, total_indices);

            // Меняем буферы
            glfwSwapBuffers(app->window);

        } else {
            break;
        }

    }

    // Очистка OpenGL данных
    clear_gl_data(render_ctx);

    // Деинициализировать CImGUI
    c_im_gui_deinit();

    // Деинициализировать GLFW
    glfw_deinit(app);

    // Деинициализировать FreeImage
    deinit_free_image();

}

#endif    // RENDER_C
