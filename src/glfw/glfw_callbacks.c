#ifndef GLFW_CALLBACKS_C
#define GLFW_CALLBACKS_C

#include "..\io_fun.c"
#include "..\window\win_utils.c"

void set_glfw_callbacks(Application * app) {

    // Привязать структуру программы к окну GLFW
    glfwSetWindowUserPointer(app->window.window_glfw, app);

    // Обработка изменения размера окна
    glfwSetFramebufferSizeCallback(app->window.window_glfw, reshape);

    // Обработка событий мыши
    glfwSetCursorPosCallback    (app->window.window_glfw, motion_fun    );
    glfwSetMouseButtonCallback  (app->window.window_glfw, mouse_fun     );
    glfwSetScrollCallback       (app->window.window_glfw, mouse_wheel   );

    // Обработка событий клавиатуры
    glfwSetKeyCallback(app->window.window_glfw, keyboard_fun);

}

#endif    // GLFW_CALLBACKS_C
