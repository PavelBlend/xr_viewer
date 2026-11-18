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
    display_draw(app->window, app);

    // Рендер интерфейса
    draw_ui(app);

    // Меняем буферы
    glfwSwapBuffers(app->window);

}

// Установить позицию камеры
void set_camera_position(Application * app) {
    pan_x =  app->render_ctx.model_center[0];
    pan_y =  app->render_ctx.model_center[1];
    pan_z = -app->render_ctx.model_center[2];
}

// Главный цикл программы
void app_main_loop(Application * app) {

    // Установить начальную позицию камеры
    set_camera_position(app);

    while (!glfwWindowShouldClose(app->window)) {

        if (app->running) {
            app_loop_iteration(app);
        } else {
            break;
        }

    }

}

#endif    // APP_LOOP_C
