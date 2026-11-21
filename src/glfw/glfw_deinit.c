#ifndef GLFW_DEINIT_C
#define GLFW_DEINIT_C

// Деинициализировать GLFW
void glfw_deinit(Application * app) {

    glfwDestroyWindow(app->window);
    glfwTerminate();

}

#endif    // GLFW_DEINIT_C
