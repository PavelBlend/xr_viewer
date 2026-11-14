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

#define IDI_ICON1 101

GLFWwindow* window = NULL;

void maximizeWindow(char* window_title) {

    // Получаем handle окна
    HWND hwnd = FindWindow(NULL, window_title);

    if (hwnd) {
        // максимизировать окно
        ShowWindow(hwnd, SW_MAXIMIZE);

        // Загружаем иконку из файла
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        if (hIcon) {
            // Устанавливаем иконку для окна
            SendMessage(hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }

    }
}

void draw_faces(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    // рандомный цвет меша
    float r, g, b;

    // отрисовка треугольников
    if (overlay_ctx->draw_faces) {

        if (render_ctx->cull_face) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // сместить полигоны в буфере глубины, чтобы не было артефактов на wireframe
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        if (render_ctx->shadeless) {
            glUniform1i(render_ctx->useShadelessLoc, 1);
        } else {
            glUniform1i(render_ctx->useShadelessLoc, 0);
        }

        for (uint32_t child_id=0; child_id<render_ctx->children_count; child_id++) {

            if (render_ctx->textures[child_id] && render_ctx->use_textures) {
                glUniform1i(render_ctx->useTextureLoc, 1);  // Включаем текстуру
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, render_ctx->textures[child_id]);
            } else {
                glUniform1i(render_ctx->useTextureLoc, 0);
                random_color_from_id(child_id, &r, &g, &b);
                glUniform3f(render_ctx->solidColorLoc, r, g, b);
            }

            glBindVertexArray(render_ctx->vao[child_id]);
            glDrawElements(GL_TRIANGLES, render_ctx->indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }

        // выключить смещение полигонов в Z буфере
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

}

void draw_edges(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    if (overlay_ctx->draw_edges) {

        if (render_ctx->cull_edge) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);

        for (uint32_t child_id=0; child_id<render_ctx->children_count; child_id++) {

            glUniform1i(render_ctx->useTextureLoc, 0);
            glUniform1i(render_ctx->useShadelessLoc, 1);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->edges_color[0], render_ctx->edges_color[1], render_ctx->edges_color[2]);

            glBindVertexArray(render_ctx->vao[child_id]);
            glDrawElements(GL_TRIANGLES, render_ctx->indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

}

void draw_grid(RenderContext * render_ctx, OverlayContext * overlay_ctx) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);

    for (uint32_t i=0; i<4; i++) {

        glUniform1i(render_ctx->useTextureLoc, 0);
        glUniform1i(render_ctx->useShadelessLoc, 1);

        if (i==0) {
            if (overlay_ctx->draw_axis_x) {    // цвет оси X
                glLineWidth(1.5f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->x_axis_color[0], render_ctx->x_axis_color[1], render_ctx->x_axis_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            } else if (overlay_ctx->draw_grid) {    // цвет сетки
                glLineWidth(1.0f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            }

        } else if (i==1 && overlay_ctx->draw_axis_y) {
            glLineWidth(1.5f);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->y_axis_color[0], render_ctx->y_axis_color[1], render_ctx->y_axis_color[2]);
            glBindVertexArray(overlay_ctx->vao[i]);
            glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);

        } else if (i==2) {
            if (overlay_ctx->draw_axis_z) {    // цвет оси Z
                glLineWidth(1.5f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->z_axis_color[0], render_ctx->z_axis_color[1], render_ctx->z_axis_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            } else if (overlay_ctx->draw_grid) {    // цвет сетки
                glLineWidth(1.0f);
                glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
                glBindVertexArray(overlay_ctx->vao[i]);
                glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
            }

        } else  if (i==3 && overlay_ctx->draw_grid) {
            glLineWidth(1.0f);
            glUniform3f(render_ctx->solidColorLoc, render_ctx->grid_color[0], render_ctx->grid_color[1], render_ctx->grid_color[2]);
            glBindVertexArray(overlay_ctx->vao[i]);
            glDrawElements(GL_LINES, overlay_ctx->indices_count[i], GL_UNSIGNED_SHORT, 0);
        }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

}

void display(GLFWwindow* window, Application* app) {

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    // включаем vsync
    if (render_ctx->vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    // Очистка экрана
    glClearColor(render_ctx->clear_color[0], render_ctx->clear_color[1], render_ctx->clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(render_ctx->ShaderProgram);

    set_shader_matrices(window, render_ctx->ShaderProgram);

    // направление света в пространстве вида (следует за камерой),
    // переведённое обратно в мир через инверсию view

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    float rad_x = glm_rad(camera_angle_x);
    float rad_y = glm_rad(-camera_angle_y);

    vec3 up = {0.0f, 1.0f, 0.0f};
    if (cosf(rad_x) < 0.0f) {
        up[1] = -1.0f;
    }

    vec3 center = {pan_x, pan_y, pan_z};
    vec3 eye = {
        pan_x + zoom * cosf(rad_x) * sinf(rad_y),
        pan_y + zoom * sinf(rad_x),
        pan_z + zoom * cosf(rad_x) * cosf(rad_y)
    };

    mat4 view;
    glm_lookat(eye, center, up, view);

    // матрица вида 3x3
    mat3 view3;
    glm_mat4_pick3(view, view3);

    float lx = -0.408163f;
    float ly = +0.844415f;
    float lz = +0.346939f;

    // направление света в пространстве вида (константа)
    vec3 light_view = { lx, ly, lz };
    glm_vec3_normalize(light_view);

    // перевод в мир: world = transpose(view3) * view
    vec3 light_world;
    glm_mat3_mulv(view3, light_view, light_world); // если cglm требует — используйте transpose
    glm_vec3_normalize(light_world);

    mat3 view3t;
    glm_mat3_transpose_to(view3, view3t);
    glm_mat3_mulv(view3t, light_view, light_world);

    glUniform3f(render_ctx->lightDirLoc, light_world[0], light_world[1], light_world[2]);

    // отрисовка треугольников
    draw_faces(render_ctx, overlay_ctx);

    // отрисовка рёбер
    draw_edges(render_ctx, overlay_ctx);

    // отрисовка координатной сетки
    draw_grid(render_ctx, overlay_ctx);

}

void clear_gl_data(RenderContext * render_ctx) {

    // Очистка ресурсов OpenGL
    for (uint32_t i = 0; i < render_ctx->children_count; i++) {
        glDeleteVertexArrays(1, &render_ctx->vao[i]);
        glDeleteBuffers(1, &render_ctx->vertex_buffers[i]);
        glDeleteBuffers(1, &render_ctx->index_buffers[i]);
        if (render_ctx->textures[i]) {
            glDeleteTextures(1, &render_ctx->textures[i]);
        }
    }

    // Освобождение выделенной памяти

    if (render_ctx->vao != NULL) {
        free(render_ctx->vao);
    }

    if (render_ctx->vertex_buffers != NULL) {
        free(render_ctx->vertex_buffers);
    }

    if (render_ctx->index_buffers != NULL) {
        free(render_ctx->index_buffers);
    }

    if (render_ctx->indices_count != NULL) {
        free(render_ctx->indices_count);
    }

    if (render_ctx->textures != NULL) {
        free(render_ctx->textures);
    }

    // Удаление шейдерной программы
    glDeleteProgram(render_ctx->ShaderProgram);

}

void reshape(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void render(Application * app) {

    // init FreeImage
    init_free_image();

    // Инициализация GLFW
    glfwInit();

    // установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // установка профиля OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // включить использование мультисемплинга через glfw
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Создать строку для заголовка окна

    char* window_title = NULL;
    char* window_name = "X-Ray Viewer";

    if (app->file_path != NULL) {

        // длина имени программы
        size_t win_name_size = strlen(window_name);

        // длина пути к файлу
        size_t file_path_size = strlen(app->file_path);

        // разделители
        char* separator_1 = " - [";
        char* separator_2 = "]";

        // длина разделителей
        size_t separator_1_size = strlen(separator_1);
        size_t separator_2_size = strlen(separator_2);

        // длина имени программы + длина разделителя + длина пути к файлу + длина второго разделителя + 1 нулевой символ
        size_t window_title_size = win_name_size + separator_1_size + file_path_size + separator_2_size + 1;

        // выделить память для строки заголовка окна
        window_title = malloc(window_title_size);

        // текущая позиция записи для строки заголовка
        char* pos = window_title;

        // скопировать имя программы в итоговую строку
        memcpy(pos, window_name, win_name_size);
        pos += win_name_size;

        // скопировать разделитель в итоговую строку
        memcpy(pos, separator_1, separator_1_size);
        pos += separator_1_size;

        // скопировать путь к файлу в итоговую строку
        memcpy(pos, app->file_path, file_path_size);
        pos += file_path_size;

        // скопировать второй разделитель в итоговую строку
        memcpy(pos, separator_2, separator_2_size);
        pos += separator_2_size;

        // добавить нулевой символ
        *pos = 0;

    } else {
        window_title = window_name;
    }

    // сгенерировать seed для генератора цвета
    init_color_seed();

    // создание окна
    window = glfwCreateWindow(800, 600, window_title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // Настройка рендера
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // включить мультисемплинг в OpenGL
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);          // Включаем отсечение задних граней
    glCullFace(GL_BACK);             // Отсекаем задние
    glFrontFace(GL_CW);

    int maximize_window = 1;

    if (maximize_window) {
        // открыть окно на весь экран
        maximizeWindow(window_title);

    } else {

        // Получаем размеры экрана
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int screen_width = mode->width;
        int screen_height = mode->height;

        int win_size_x = screen_width * 0.8;
        int win_size_y = screen_height * 0.8;

        // Устанавливаем размер окна
        glfwSetWindowSize(window, win_size_x, win_size_y);

        // центрируем окно
        int pos_x = (screen_width - win_size_x) / 2;
        int pos_y = (screen_height - win_size_y) / 2;
        glfwSetWindowPos(window, pos_x, pos_y);

    }

    // Инициализация GLEW
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        printf("GLEW Init Error!\n");
    }

    // Режим заливки полигонов
    glPolygonMode(GL_FRONT, GL_FILL);

    // Контекст рендера
    RenderContext * render_ctx = &app->render_ctx;

    // Контекст наложений
    OverlayContext * overlay_ctx = &app->overlay_ctx;

    overlay_ctx->vao            = NULL;
    overlay_ctx->vertex_buffers = NULL;
    overlay_ctx->index_buffers  = NULL;
    overlay_ctx->indices_count  = NULL;

    overlay_ctx->grid_steps     = 10;
    overlay_ctx->grid_size      = 1.0f;

    overlay_ctx->draw_faces     = true;
    overlay_ctx->draw_edges     = false;
    overlay_ctx->draw_grid      = true;
    overlay_ctx->draw_axis_x    = true;
    overlay_ctx->draw_axis_y    = true;
    overlay_ctx->draw_axis_z    = true;

    // Генерация координатной сетки
    generate_grid(overlay_ctx);

    render_ctx->vsync = true;

    // параметры шейдинга
    render_ctx->shadeless       = false;
    render_ctx->use_textures    = true;

    // Установить параметры отсечения
    render_ctx->cull_face = true;
    render_ctx->cull_edge = false;

    // Установить цвет для очистки экрана
    render_ctx->clear_color[0] = 0.5f;
    render_ctx->clear_color[1] = 0.5f;
    render_ctx->clear_color[2] = 0.5f;
    glClearColor(render_ctx->clear_color[0], render_ctx->clear_color[1], render_ctx->clear_color[2], 1.0);

    // Установить цвет рёбер меша
    render_ctx->edges_color[0] = 0.0f;
    render_ctx->edges_color[1] = 0.0f;
    render_ctx->edges_color[2] = 0.0f;

    // Установить цвет координатной сетки
    render_ctx->grid_color[0] = 0.45f;
    render_ctx->grid_color[1] = 0.45f;
    render_ctx->grid_color[2] = 0.45f;

    // Установить цвет оси X
    render_ctx->x_axis_color[0] = 0.5f;
    render_ctx->x_axis_color[1] = 0.0f;
    render_ctx->x_axis_color[2] = 0.0f;

    // Установить цвет оси Y
    render_ctx->y_axis_color[0] = 0.0f;
    render_ctx->y_axis_color[1] = 0.5f;
    render_ctx->y_axis_color[2] = 0.0f;

    // Установить цвет оси Z
    render_ctx->z_axis_color[0] = 0.0f;
    render_ctx->z_axis_color[1] = 0.0f;
    render_ctx->z_axis_color[2] = 0.5f;

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

    // Привязать структуру программы к окну GLFW
    glfwSetWindowUserPointer(window, app);

    // обработка изменения размера окна
    glfwSetFramebufferSizeCallback(window, reshape);

    // Обработка событий мыши
    glfwSetCursorPosCallback(window, motion_fun);
    glfwSetMouseButtonCallback(window, mouse_fun);
    glfwSetScrollCallback(window, mouse_wheel);

    // Обработка событий клавиатуры
    glfwSetKeyCallback(window, keyboard_fun);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    // Инициализация CImGUI
    c_im_gui_init(window, app->exe_dir);

    // Переменные для подсчёта FPS
    static double last_time = 0.0;
    static double fps = 0.0;

    // общая строка символов для меток в интерфейсе
    char buffer[15];

    // установить начальную позицию камеры
    pan_x = render_ctx->model_center[0];
    pan_y = render_ctx->model_center[1];
    pan_z = -render_ctx->model_center[2];

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {

        if (app->running) {

            // Обработка событий
            glfwPollEvents();

            // рендер сцены
            display(window, app);

            // рендер интерфейса
            draw_ui(app, buffer, &fps, &last_time, total_vertices, total_indices);

            // Меняем буферы
            glfwSwapBuffers(window);

        } else {
            break;
        }

    }

    // очистка OpenGL данных
    clear_gl_data(render_ctx);

    // деинициализировать cimgui
    c_im_gui_deinit();

    glfwDestroyWindow(window);
    glfwTerminate();

    // deinit FreeImage
    deinit_free_image();

}

#endif    // RENDER_C
