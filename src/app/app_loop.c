#ifndef APP_LOOP_C
#define APP_LOOP_C

#include "..\globals.c"
#include "..\display\display_draw.c"
#include "..\ui\im_gui_draw.c"

// Итерация цикла программы
void app_loop_iteration(Application * app) {

    // Обработка событий
    glfwPollEvents();

    // Рендер сцены
    display_draw(app->window.window_glfw, app);

    // Рендер интерфейса
    draw_ui(app);

    // Меняем буферы
    glfwSwapBuffers(app->window.window_glfw);

}

// Установить позицию камеры
void set_camera_position(Application * app) {

    OgfFile *ogf = (OgfFile*)app->file;

    if (ogf == NULL || *ogf->header.bsphere_radius <= 0.0f) {
        zoom = 3.0f;   // значение по умолчанию
        return;
    }

    // установить позицию камеры
    pan_x =  *ogf->header.bsphere_center[0];
    pan_y =  *ogf->header.bsphere_center[1];
    pan_z = -*ogf->header.bsphere_center[2];

    // вычислить aspect
    int width, height;
    glfwGetWindowSize(app->window.window_glfw, &width, &height);

    float aspect;
    if (height == 0) {
        aspect = 1.0f;
    } else {
        aspect = ((float)width - 350.0f) / (float)height;
        if (aspect <= 0.0f) {
            aspect = 1.0f;
        }
    }

    // fov
    const float PI = 3.14159265358979323846f;
    float vfov = fov * PI / 180.0f;
    float hfov = 2.0f * atanf(tanf(vfov * 0.5f) * aspect);

    // вычислить расстояние
    float d_v = *ogf->header.bsphere_radius / sinf(vfov * 0.5f);
    float d_h = *ogf->header.bsphere_radius / sinf(hfov * 0.5f);

    // добавить 15% запаса
    zoom = fmaxf(d_v, d_h) * 1.15f;

}

// Главный цикл программы
void app_main_loop(Application * app) {

    // Установить начальную позицию камеры
    set_camera_position(app);

    while (!glfwWindowShouldClose(app->window.window_glfw)) {

        if (app->running) {
            app_loop_iteration(app);
        } else {
            break;
        }

    }

}

#endif    // APP_LOOP_C
