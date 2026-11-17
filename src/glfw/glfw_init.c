#ifndef GLFW_INIT_C
#define GLFW_INIT_C

#include "GLFW\glfw3.h"

void glfw_init(void) {

    // Инициализация GLFW
    glfwInit();

    // установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // установка профиля OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // включить использование мультисемплинга
    glfwWindowHint(GLFW_SAMPLES, 8);

}

#endif    // GLFW_INIT_C
