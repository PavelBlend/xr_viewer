#ifndef LEVEL_TYPES_H
#define LEVEL_TYPES_H

#define SUPPORTED_LEVEL_VERSION 14

typedef struct LevelHeader {
    uint16_t *      format_version;
    uint16_t *      quality;
} LevelHeader;

typedef struct LevelShaders {
    uint32_t *      count;              // Количество шейдеров
    char **         abs_tex_paths;      // Абсолютные пути к текстурам
} LevelShaders;

typedef struct LevelFile {
    LevelHeader     header;
    LevelShaders    shaders;
    FileData *      geom_data;
} LevelFile;

typedef struct LevelGeomFile {
    LevelHeader     header;
} LevelGeomFile;

#endif    // LEVEL_TYPES_H
