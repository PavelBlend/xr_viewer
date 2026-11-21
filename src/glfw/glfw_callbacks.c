#ifndef GLFW_CALLBACKS_C
#define GLFW_CALLBACKS_C

#include "..\io_fun.c"
#include "..\window\win_utils.c"

void set_glfw_callbacks(Application * app) {

    // Привязать структуру программы к окну GLFW
    glfwSetWindowUserPointer(app->window, app);

    // Обработка изменения размера окна
    glfwSetFramebufferSizeCallback(app->window, reshape);

    // Обработка событий мыши
    glfwSetCursorPosCallback    (app->window, motion_fun    );
    glfwSetMouseButtonCallback  (app->window, mouse_fun     );
    glfwSetScrollCallback       (app->window, mouse_wheel   );

    // Обработка событий клавиатуры
    glfwSetKeyCallback(app->window, keyboard_fun);

}

#endif    // GLFW_CALLBACKS_C
