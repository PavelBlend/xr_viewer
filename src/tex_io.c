#ifndef TEX_IO_C
#define TEX_IO_C

// Загрузить dds текстуру
GLuint load_dds_texture(const char* filename) {

    GLuint textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,             // Автоматически определять формат
        SOIL_CREATE_NEW_ID,         // Создать новый ID текстуры
        SOIL_FLAG_DDS_LOAD_DIRECT   // Прямая загрузка сжатого DDS
    );

    // Проверяем результат загрузки
    if (textureID == 0) {
        printf("Cannot load DDS texture: %s\n", filename);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Устанавливаем параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR);

    return textureID;
}

#endif    // TEX_IO_C
