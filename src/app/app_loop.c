#ifndef APP_LOOP_C
#define APP_LOOP_C

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

// Главный цикл программы
void app_main_loop(Application * app) {

    while (!glfwWindowShouldClose(app->window)) {

        if (app->running) {
            app_loop_iteration(app);
        } else {
            break;
        }

    }

}

#endif    // APP_LOOP_C
