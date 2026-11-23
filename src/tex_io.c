#ifndef TEX_IO_C
#define TEX_IO_C

typedef struct TexCacheEntry {
    char*   path;
    GLuint  id;
} TexCacheEntry;

static TexCacheEntry* g_tex_cache = NULL;
static int            g_tex_count = 0;
static int            g_tex_cap   = 0;

// Ищет уже загруженную текстуру по пути. Возвращает 0, если нет.
static GLuint cache_lookup(const char* path) {
    for (int i = 0; i < g_tex_count; ++i) {
        if (strcmp(g_tex_cache[i].path, path) == 0)
            return g_tex_cache[i].id;
    }
    return 0;
}

// Кладёт результат в кеш. Возвращает 0 при успехе, -1 при ошибке памяти.
static int cache_insert(const char* path, GLuint id) {

    if (g_tex_count == g_tex_cap) {
        int new_cap = g_tex_cap ? g_tex_cap * 2 : 16;
        TexCacheEntry* p = (TexCacheEntry*)realloc(g_tex_cache, new_cap * sizeof(*p));
        if (!p) {
            return -1;
        }
        g_tex_cache = p;
        g_tex_cap   = new_cap;
    }

    char* copy = (char*)malloc(strlen(path) + 1);
    if (!copy) {
        return -1;
    }
    strcpy(copy, path);

    g_tex_cache[g_tex_count].path = copy;
    g_tex_cache[g_tex_count].id   = id;
    g_tex_count++;

    return 0;
}

GLuint load_dds_texture(const char* filename) {

    // 1. Сначала ищем в кеше
    GLuint cached = cache_lookup(filename);
    if (cached != 0)
        return cached;

    // 2. Если нет — грузим
    GLuint textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,             // Автоматически определять формат
        SOIL_CREATE_NEW_ID,         // Создать новый ID текстуры
        SOIL_FLAG_DDS_LOAD_DIRECT   // Прямая загрузка сжатого DDS
    );

    if (textureID == 0) {
        printf("Cannot load DDS texture: %s\n", filename);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 3. Запоминаем
    cache_insert(filename, textureID);

    return textureID;
}

// Освобождение кеша
void tex_cache_free(void) {

    for (int i = 0; i < g_tex_count; ++i)
        free(g_tex_cache[i].path);

    free(g_tex_cache);

    g_tex_cache = NULL;
    g_tex_count = 0;
    g_tex_cap   = 0;

}

#endif    // TEX_IO_C
