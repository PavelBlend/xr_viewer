#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <windows.h>
#include <math.h>

#include "cimgui.h"
#include "cimgui_impl.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "cglm\cglm.h"
#include "FreeImage.h"

#include "render.h"
#include "io_fun.c"
#include "globals.c"
#include "tex_io.c"
#include "shaders.c"
#include "overlays.c"
#include "formating.c"
#include "ui\im_gui_init.c"
#include "ui\im_gui_deinit.c"
#include "ui\im_gui_draw.c"

#define IDI_ICON1 101

char* window_title = "X-Ray Viewer";
GLFWwindow* window = NULL;

// Создать контекст рендера
RenderContext ctx;
OverlayContext overlay_ctx;

void maximizeWindow() {
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

void display() {
    // Очистка экрана
    glClearColor(ctx.clear_color[0], ctx.clear_color[1], ctx.clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ctx.ShaderProgram);

    set_shader_matrices(window, ctx.ShaderProgram);

    // отрисовка треугольников
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (uint32_t child_id=0; child_id<ctx.children_count; child_id++) {

        if (ctx.textures[child_id]) {
            glUniform1i(ctx.useTextureLoc, 1);  // Включаем текстуру
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ctx.textures[child_id]);
        } else {
            glUniform1i(ctx.useTextureLoc, 0);
            glUniform3f(ctx.solidColorLoc, 1.0f, 1.0f, 1.0f);
        }

        glBindVertexArray(ctx.vao[child_id]);
        glDrawElements(GL_TRIANGLES, ctx.indices_count[child_id], GL_UNSIGNED_SHORT, 0);

    }

    // отрисовка рёбер
    if (draw_edges) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0f, -1.0f);
        glLineWidth(1.0f);

        for (uint32_t child_id=0; child_id<ctx.children_count; child_id++) {

            glUniform1i(ctx.useTextureLoc, 0);
            glUniform3f(ctx.solidColorLoc, ctx.edges_color[0], ctx.edges_color[1], ctx.edges_color[2]);

            glBindVertexArray(ctx.vao[child_id]);
            glDrawElements(GL_TRIANGLES, ctx.indices_count[child_id], GL_UNSIGNED_SHORT, 0);

        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);

    }

    // отрисовка координатной сетки
    if (1) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);

        for (uint32_t i=0; i<1; i++) {

            glUniform1i(ctx.useTextureLoc, 0);
            glUniform3f(ctx.solidColorLoc, 0.25f, 0.25f, 0.25f);

            glBindVertexArray(overlay_ctx.vao[i]);
            glDrawElements(GL_LINES, overlay_ctx.indices_count[i], GL_UNSIGNED_SHORT, 0);

        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

    // render_overlays(ctx);

}

void clear_gl_data(void) {

    // Очистка ресурсов OpenGL
    for (uint32_t i = 0; i < ctx.children_count; i++) {
        glDeleteVertexArrays(1, &ctx.vao[i]);
        glDeleteBuffers(1, &ctx.vertex_buffers[i]);
        glDeleteBuffers(1, &ctx.index_buffers[i]);
        if (ctx.textures[i]) {
            glDeleteTextures(1, &ctx.textures[i]);
        }
    }

    // Освобождение выделенной памяти
    free(ctx.vao);
    free(ctx.vertex_buffers);
    free(ctx.index_buffers);
    free(ctx.indices_count);
    free(ctx.textures);

    // Удаление шейдерной программы
    glDeleteProgram(ctx.ShaderProgram);

}

void reshape(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void render(OgfFile* ogf_file, int argc, char** argv, char* exe_dir) {

    // init FreeImage
    init_free_image();

    // Инициализация GLFW
    glfwInit();

    // установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // установка профиля OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // включить использование мультисемплинга через glfw
    glfwWindowHint(GLFW_SAMPLES, 8);

    // создание окна
    window = glfwCreateWindow(800, 600, window_title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // включаем vsync
    glfwSwapInterval(1);

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
        maximizeWindow();

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









    // Настроить overlay контекст
    overlay_ctx.vao = malloc(sizeof(GLuint) * 4);
    overlay_ctx.vertex_buffers = malloc(sizeof(GLuint) * 4);
    overlay_ctx.grid_steps = 10;
    overlay_ctx.grid_size = 1.0f;

    // Вычислить количество линий координатной сетки:
    // overlay_ctx.grid_steps - количество линий для половины сетки.
    // Всего два вида линий: параллельно оси X и параллельно оси Z, поэтому умножаем на 2.
    // Все линии располагаются в положительную и в отрицательную сторону оси, поэтому ещё умножаем на 2.
    uint32_t lines_count = overlay_ctx.grid_steps * 2 * 2;

    // Вычислить количество вершин:
    // Количество линий нужно умножить на 2, так как одна линия строится на друх вершинах.
    uint32_t grid_verts_count = lines_count * 2;

    // Вычислить размер вершин сетки:
    // Вершина - это вектор с тремя элементами X, Y, Z с типом float.
    size_t grid_verts_size = sizeof(float) * 3 * grid_verts_count;

    // Выделяем память для вершин сетки
    float* grid_vertices = malloc(grid_verts_size);

    uint32_t vert_index = 0;

    // Линии X
    for (int32_t i = -((int32_t)overlay_ctx.grid_steps); i <= (int32_t)overlay_ctx.grid_steps; i++) {
        if (i != 0) {    // пропускаем линии в нулевых координатах
            float pos = i * overlay_ctx.grid_size;

            // Две вершины линии
            grid_vertices[vert_index + 0] = -overlay_ctx.grid_size * overlay_ctx.grid_steps;
            grid_vertices[vert_index + 1] = 0.0f;
            grid_vertices[vert_index + 2] = pos;

            grid_vertices[vert_index + 3] = +overlay_ctx.grid_size * overlay_ctx.grid_steps;
            grid_vertices[vert_index + 4] = 0.0f;
            grid_vertices[vert_index + 5] = pos;

            vert_index += 6;
        }
    }

    // Линии Z
    for (int i = -((int32_t)overlay_ctx.grid_steps); i <= (int32_t)overlay_ctx.grid_steps; i++) {
        if (i != 0) {    // пропускаем линии в нулевых координатах
            float pos = i * overlay_ctx.grid_size;

            // Две вершины линии
            grid_vertices[vert_index + 0] = pos;
            grid_vertices[vert_index + 1] = 0.0f;
            grid_vertices[vert_index + 2] = -overlay_ctx.grid_size * overlay_ctx.grid_steps;

            grid_vertices[vert_index + 3] = pos;
            grid_vertices[vert_index + 4] = 0.0f;
            grid_vertices[vert_index + 5] = +overlay_ctx.grid_size * overlay_ctx.grid_steps;

            vert_index += 6;
        }
    }

    // Создать вершинный буфер для координатной сетки
    glGenVertexArrays(1, &overlay_ctx.vao[0]);
    glBindVertexArray(overlay_ctx.vao[0]);

    glGenBuffers(1, &overlay_ctx.vertex_buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, overlay_ctx.vertex_buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, grid_verts_size, grid_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0);

    // Создать индексный буфер для координатной сетки
    overlay_ctx.index_buffers = malloc(sizeof(GLuint) * 4);
    overlay_ctx.indices_count = malloc(sizeof(uint32_t) * 4);

    uint32_t grid_indices_count = lines_count * 2;
    size_t grid_indices_size = sizeof(uint16_t) * (size_t)grid_indices_count;

    uint16_t* grid_indices = malloc(grid_indices_size);

    // генерировать индексы координатной сетки
    uint16_t current_index = 0;
    for (uint16_t i=0; i<grid_indices_count; i++) {
        grid_indices[i] = current_index;
        current_index++;
    }

    glGenBuffers(1, &overlay_ctx.index_buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_ctx.index_buffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid_indices_size, grid_indices, GL_STATIC_DRAW);

    overlay_ctx.indices_count[0] = grid_indices_count;











    // Установить цвет для очистки экрана
    ctx.clear_color[0] = 0.5f;
    ctx.clear_color[1] = 0.5f;
    ctx.clear_color[2] = 0.5f;
    glClearColor(ctx.clear_color[0], ctx.clear_color[1], ctx.clear_color[2], 1.0);

    // Установить цвет рёбер меша
    ctx.edges_color[0] = 1.0f;
    ctx.edges_color[1] = 1.0f;
    ctx.edges_color[2] = 1.0f;

    // загрузить шейдеры
    ctx.ShaderProgram = compile_shaders(exe_dir);

    ctx.useTextureLoc = glGetUniformLocation(ctx.ShaderProgram, "useTexture");
    ctx.solidColorLoc = glGetUniformLocation(ctx.ShaderProgram, "solidColor");

    // статистика
    uint32_t total_vertices = 0;
    uint32_t total_indices = 0;

    if (ogf_file != NULL) {
        ctx.vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.vertex_buffers  = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.index_buffers   = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        ctx.textures        = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.children_count  = ogf_file->children_count;

        for (uint32_t child_id=0; child_id<ogf_file->children_count; child_id++) {

            // Создать VAO
            glGenVertexArrays(1, &ctx.vao[child_id]);
            glBindVertexArray(ctx.vao[child_id]);

            // Создать вершинный буфер
            glGenBuffers(1, &ctx.vertex_buffers[child_id]);
            glBindBuffer(GL_ARRAY_BUFFER, ctx.vertex_buffers[child_id]);

            size_t verts_size;

            OgfChild* ogf_child = &ogf_file->children[child_id];

            total_vertices += ogf_child->verts->count;
            total_indices += ogf_child->indices->count;

            if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
                verts_size = ogf_child->verts->count * 60;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_1_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 60, (void*) 0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 60, (void*) 48);    // uv attribute

            } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
                verts_size = ogf_child->verts->count * 64;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_2_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 64, (void*) 4);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 64, (void*) 56);    // uv attribute

            } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
                verts_size = ogf_child->verts->count * 70;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_3_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 70, (void*) 6);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 70, (void*) 62);    // uv attribute

            } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
                verts_size = ogf_child->verts->count * 76;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_4_link, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 76, (void*) 8);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 76, (void*) 68);    // uv attribute

            } else if (ogf_child->verts->format == FVF_OGF) {
                verts_size = ogf_child->verts->count * 32;
                glBufferData(GL_ARRAY_BUFFER, verts_size, ogf_child->verts->verts_static, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*) 0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*) 24);    // uv attribute

            } else {
                printf("unsupported rendering for vertex format: 0x%x\n", ogf_child->verts->format);
                exit(EXIT_FAILURE);
            }

            // Настройка атрибута позиции
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            // Создать буфер индексов
            glGenBuffers(1, &ctx.index_buffers[child_id]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.index_buffers[child_id]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * ogf_child->indices->count, ogf_child->indices->indices, GL_STATIC_DRAW);

            ctx.indices_count[child_id] = ogf_child->indices->count;

            // Загрузить текстуру
            ctx.textures[child_id] = load_dds_texture(ogf_child->texture_path);
            // printf("Load Texture: \"%s\"\n", ogf_child->texture_path);

        }

    } else {
        ctx.children_count = 0;
    }

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
    c_im_gui_init(window, exe_dir);

    // Переменные для подсчёта FPS
    static double last_time = 0.0;
    static double fps = 0.0;

    // общая строка символов для меток в интерфейсе
    char buffer[15];

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {

        // Обработка событий
        glfwPollEvents();

        // рендер сцены
        display();

        // рендер интерфейса
        draw_ui(&ctx, buffer, &fps, &last_time, total_vertices, total_indices);

        // Меняем буферы
        glfwSwapBuffers(window);

    }

    // очистка OpenGL данных
    clear_gl_data();

    // деинициализировать cimgui
    c_im_gui_deinit();

    glfwDestroyWindow(window);
    glfwTerminate();

    // deinit FreeImage
    deinit_free_image();

}
