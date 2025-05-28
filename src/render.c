#include <windows.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "cglm\cglm.h"
#include "FreeImage.h"
#include "io_fun.c"
#include "globals.c"

#define IDI_ICON1 101

typedef struct RenderContext {
    GLuint*     vao;
    uint32_t*   indices_count;
    uint32_t    children_count;
} RenderContext;

// Создать контекст рендера
RenderContext ctx;

void maximizeWindow() {
    // Получаем handle окна FreeGLUT
    HWND hwnd = FindWindow(NULL, "OGF Viewer");

    if (hwnd) {
        // максимизировать окно
        ShowWindow(hwnd, SW_MAXIMIZE);

        // Загружаем иконку из файла
        //HICON hIcon = (HICON)LoadImage(NULL, "ogf.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
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
    glClear(GL_COLOR_BUFFER_BIT);

    // Устанавливаем матрицу вида
    glLoadIdentity();
    glScalef(zoom, zoom, zoom);    // Применяем масштаб
    glTranslatef(pan_x, pan_y, 0.0f); // Применяем панорамирование
    glRotatef(camera_angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(camera_angle_y, 0.0f, 1.0f, 0.0f);
    glScalef(1.0, 1.0, -1.0);    // Инверсия оси Z для перехода от DirectX к OpenGL

    for (uint32_t child_id=0; child_id<ctx.children_count; child_id++) {
        glBindVertexArray(ctx.vao[child_id]);
        glDrawElements(GL_TRIANGLES, ctx.indices_count[child_id], GL_UNSIGNED_SHORT, 0);
    }

    // Обновление окна
    glutSwapBuffers();
}

void reshape(int width, int height) {
    // Избегаем деления на ноль
    if (height == 0) height = 1;

    // Соотношение сторон окна
    float aspect = (float)width / (float)height;

    // Устанавливаем область просмотра
    glViewport(0, 0, width, height);

    // Выбираем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float vertical_offset = 0.5;

    // Ортографическая проекция или перспектива
    if (aspect >= 1.0f) {
        // Ширина больше или равна высоте, корректируем по ширине
        glOrtho(-aspect, aspect, -1.0 + vertical_offset, 1.0 + vertical_offset, -10.0, 10.0);
    } else {
        // Высота больше ширины, корректируем по высоте
        glOrtho(-1.0, 1.0, -1.0 / aspect + vertical_offset, 1.0 / aspect + vertical_offset, -10.0, 10.0);
    }

    // Возвращаемся к модели
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render(OgfFile* ogf_file, int argc, char** argv) {

    // Инициализация FreeGLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Настройка рендера
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Создание окна
    glutCreateWindow("OGF Viewer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    int maximize_window = 1;

    if (maximize_window) {
        // открыть окно на весь экран
        maximizeWindow();

    } else {
        // Получение разрешения экрана
        int screen_width = glutGet(GLUT_SCREEN_WIDTH);
        int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

        int win_size_x = screen_width * 0.8;
        int win_size_y = screen_height * 0.8;
        glutReshapeWindow(win_size_x, win_size_y);
        glutPositionWindow(
            (screen_width - win_size_x) / 2,
            (screen_height - win_size_y) / 2
        );
    }

    // Инициализация GLEW
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        printf("GLEW Init Error!\n");
    }

    // Режим заливки полигонов
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Установить цвет для очистки экрана
    glClearColor(0.5, 0.5, 0.5, 1.0);

    ctx.vao             = malloc(sizeof(GLuint)     * ogf_file->children_count);
    ctx.indices_count   = malloc(sizeof(uint32_t)   * ogf_file->children_count);
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

    }

    // Обработка событий мыши
    glutMouseFunc(mouse_fun);
    glutMotionFunc(motion_fun);
    glutMouseWheelFunc(mouse_wheel);

    // Обработка событий клавиатуры
    glutKeyboardFunc(keyboard_fun);

    // Главный цикл OpenGL
    glutMainLoop(); 
}
