#ifndef OPENGL_SETTINGS_C
#define OPENGL_SETTINGS_C

#include "GL\glew.h"

// Установить настройки рендера
void set_opengl_render_settings(void) {

    // Включить тест глубины
    glEnable(GL_DEPTH_TEST);

    // Выбрать функцию теста глубины
    glDepthFunc(GL_LESS);

    // Включить мультисемплинг
    glEnable(GL_MULTISAMPLE);

    // Включить отсечение задних граней
    glEnable(GL_CULL_FACE);

    // Отсекаем задние
    glCullFace(GL_BACK);

    // Установить порядок обхода лицевых граней
    glFrontFace(GL_CW);

}

#endif    // OPENGL_SETTINGS_C
