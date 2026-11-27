#pragma once

// Деинициализировать GLFW
void glfw_deinit(Application * app) {

    glfwDestroyWindow(app->window.window_glfw);
    glfwTerminate();

}
