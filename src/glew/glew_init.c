#pragma once

// Инициализировать GLEW
void glew_init(void) {

    // Инициализация GLEW
    GLenum glewStatus = glewInit();

    // Проверить статус инициализации
    if (glewStatus != GLEW_OK) {
        printf("GLEW Init Error!\n");
        exit(-1);
    }

}
