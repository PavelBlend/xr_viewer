#ifndef GLFW_DEINIT_C
#define GLFW_DEINIT_C

#include "GLFW\glfw3.h"

// Деинициализировать GLFW
void glfw_deinit(Application * app) {

    glfwDestroyWindow(app->window);
    glfwTerminate();

}

#endif    // GLFW_DEINIT_C
