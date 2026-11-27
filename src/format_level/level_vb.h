#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "level_types.h"

// ---- Константы формата level ----
#define LEVEL_VB_VERSION_10 10
#define LEVEL_VB_VERSION_14 14

#define LEVEL_VB_UV_COEFFICIENT        1024.0f
#define LEVEL_VB_UV_COEFFICIENT_2      2048.0f
#define LEVEL_VB_LIGHT_MAP_COEFFICIENT 32767.0f

#define LEVEL_VB_UV_CORRECTOR_SCALE    ((float)(32.0 / 0x8000))   // 32/32768

// ---- D3D9 declaration types ----
enum LevelVBType {
    LEVEL_VB_TYPE_FLOAT2   = 1,
    LEVEL_VB_TYPE_FLOAT3   = 2,
    LEVEL_VB_TYPE_FLOAT4   = 3,
    LEVEL_VB_TYPE_D3DCOLOR = 4,
    LEVEL_VB_TYPE_SHORT2   = 6,
    LEVEL_VB_TYPE_SHORT4   = 7,
    LEVEL_VB_TYPE_UNUSED   = 17,
};

// ---- D3D9 usages ----
enum LevelVBUsage {
    LEVEL_VB_USAGE_POSITION     = 0,
    LEVEL_VB_USAGE_BLENDWEIGHT  = 1,
    LEVEL_VB_USAGE_BLENDINDICES = 2,
    LEVEL_VB_USAGE_NORMAL       = 3,
    LEVEL_VB_USAGE_PSIZE        = 4,
    LEVEL_VB_USAGE_TEXCOORD     = 5,
    LEVEL_VB_USAGE_TANGENT      = 6,
    LEVEL_VB_USAGE_BINORMAL     = 7,
    LEVEL_VB_USAGE_TESSFACTOR   = 8,
    LEVEL_VB_USAGE_POSITIONT    = 9,
    LEVEL_VB_USAGE_COLOR        = 10,
    LEVEL_VB_USAGE_FOG          = 11,
    LEVEL_VB_USAGE_DEPTH        = 12,
    LEVEL_VB_USAGE_SAMPLE       = 13,
};

typedef struct LevelVBUsageInfo {
    uint16_t stream;
    uint16_t offset;
    uint8_t  type;
    uint8_t  method;
    uint8_t  usage;
    uint8_t  usage_index;
} LevelVBUsageInfo;

typedef struct LevelVertexBuffer {
    // Геометрия
    float * positions;      // 3 * count
    float * normals;        // 3 * count

    // UV
    float * uvs;            // 2 * count
    float * uv_lmaps;       // 2 * count

    // Цвета
    float * color_hemi;     // 1 * count
    float * color_light;    // 3 * count (r,g,b)
    float * color_sun;      // 1 * count

    uint32_t count;

    bool float_normals;
    bool has_positions;
    bool has_normals;
    bool has_uvs;
    bool has_uv_lmaps;
    bool has_colors;
    bool has_hemi;
    bool has_sun;
} LevelVertexBuffer;
