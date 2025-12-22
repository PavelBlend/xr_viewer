#ifndef TEX_IO_C
#define TEX_IO_C

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <FreeImage.h>

// Инициализация FreeImage
void init_free_image() {
    FreeImage_Initialise(TRUE);
}

void deinit_free_image() {
    FreeImage_DeInitialise();
}

GLuint load_dds_texture(char* filename) {

    // Загрузка изображения DDS
    FIBITMAP* bitmap = FreeImage_Load(FIF_DDS, filename, DDS_DEFAULT);
    if (!bitmap) {
        printf("Cannot load DDS texture: %s\n", filename);
        deinit_free_image();
        return 0;
    }

    // Получение информации о текстуре
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    int bpp = FreeImage_GetBPP(bitmap);
    BYTE* data = FreeImage_GetBits(bitmap);

    // Проверка формата
    GLenum format;
    if (bpp == 24) {
        format = GL_BGR;
    } else if (bpp == 32) {
        format = GL_BGRA;
    } else {
        printf("Unsupported image format\n");
        FreeImage_Unload(bitmap);
        deinit_free_image();
        return 0;
    }

    // Создание текстуры в OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Установка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR);

    // Очистка ресурсов
    FreeImage_Unload(bitmap);

    return textureID;
}

#endif    // TEX_IO_C
