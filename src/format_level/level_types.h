#pragma once

#define SUPPORTED_LEVEL_VERSION 14

#include "level_vb.h"
#include "level_ib.h"
#include "level_swis.h"

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
    LevelSWIBuffer * swis;
    uint32_t         swis_count;
} LevelFile;

typedef struct LevelGeomFile {
    LevelHeader     header;
} LevelGeomFile;
