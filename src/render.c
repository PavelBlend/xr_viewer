#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <windows.h>

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

#define IDI_ICON1 101

char* window_title = "X-Ray Viewer";
GLFWwindow* window = NULL;

// Создать контекст рендера
RenderContext ctx;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ctx.ShaderProgram);

    set_shader_matrices(window, ctx.ShaderProgram);

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

    render_overlays(ctx);

}

void reshape(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void render(OgfFile* ogf_file, int argc, char** argv, char* exe_path) {

    // Инициализация GLFW
    glfwInit();

    // установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // установка профиля OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Установить цвет для очистки экрана
    glClearColor(0.5, 0.5, 0.5, 1.0);

    // загрузить шейдеры
    ctx.ShaderProgram = compile_shaders(exe_path);

    ctx.useTextureLoc = glGetUniformLocation(ctx.ShaderProgram, "useTexture");
    ctx.solidColorLoc = glGetUniformLocation(ctx.ShaderProgram, "solidColor");

    // статистика
    uint32_t total_vertices = 0;
    uint32_t total_indices = 0;

    if (ogf_file != NULL) {
        ctx.vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
        ctx.textures        = malloc(sizeof(GLuint)     * ogf_file->children_count);
        ctx.children_count  = ogf_file->children_count;

        for (uint32_t child_id=0; child_id<ogf_file->children_count; child_id++) {

            // Создать VAO
            glGenVertexArrays(1, &ctx.vao[child_id]);
            glBindVertexArray(ctx.vao[child_id]);

            // Создать вершинный буфер
            GLuint vertex_buffer;
            glGenBuffers(1, &vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

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
            GLuint indices_buffer;
            glGenBuffers(1, &indices_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
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
    igCreateContext(NULL);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    igStyleColorsLight(NULL);

    // Главный цикл
    while (!glfwWindowShouldClose(window))
    {
        // Обработка событий
        glfwPollEvents();

        // рендер сцены
        display();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        igNewFrame();

        igBegin("Info", NULL, 0);
        igText("Children Count: %d", ctx.children_count);
        igText("Vertices Count: %d", total_vertices);
        igText("Triangles Count: %d", total_indices / 3);
        igEnd();

        igRender();
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

        // Меняем буферы
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

    glfwDestroyWindow(window);
    glfwTerminate();

}
