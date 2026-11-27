#pragma once

#include "level_types.h"
#include "level_vb.h"
#include "..\chunk_io.c"

typedef struct LevelVBReader {
    const uint8_t * data;
    uint32_t        size;
    uint32_t        pos;
} LevelVBReader;

static void vb_reader_init(LevelVBReader * r, const uint8_t * data, uint32_t size) {
    r->data = data;
    r->size = size;
    r->pos  = 0;
}

static void vb_reader_require(LevelVBReader * r, uint32_t bytes) {
    if (r->pos + bytes > r->size) {
        printf("level.geom VB reader out of bounds: pos=%u, need=%u, size=%u\n", r->pos, bytes, r->size);
        exit(EXIT_FAILURE);
    }
}

static uint8_t vb_reader_u8(LevelVBReader * r) {
    vb_reader_require(r, 1);
    return r->data[r->pos++];
}

static uint16_t vb_reader_u16(LevelVBReader * r) {
    vb_reader_require(r, 2);
    uint16_t v;
    memcpy(&v, r->data + r->pos, 2);
    r->pos += 2;
    return v;
}

static int16_t vb_reader_i16(LevelVBReader * r) {
    vb_reader_require(r, 2);
    int16_t v;
    memcpy(&v, r->data + r->pos, 2);
    r->pos += 2;
    return v;
}

static uint32_t vb_reader_u32(LevelVBReader * r) {
    vb_reader_require(r, 4);
    uint32_t v;
    memcpy(&v, r->data + r->pos, 4);
    r->pos += 4;
    return v;
}

static float vb_reader_f32(LevelVBReader * r) {
    vb_reader_require(r, 4);
    float v;
    memcpy(&v, r->data + r->pos, 4);
    r->pos += 4;
    return v;
}

// Чтение D3D9 declaration
static LevelVBUsageInfo * read_d3d9_declaration(LevelVBReader * r, uint32_t * out_count) {

    uint32_t capacity = 16;
    uint32_t count    = 0;
    LevelVBUsageInfo * list = malloc(sizeof(LevelVBUsageInfo) * capacity);

    while (true) {
        LevelVBUsageInfo info;

        info.stream      = vb_reader_u16(r);
        info.offset      = vb_reader_u16(r);
        info.type        = vb_reader_u8(r);
        info.method      = vb_reader_u8(r);
        info.usage       = vb_reader_u8(r);
        info.usage_index = vb_reader_u8(r);

        printf("stream = %hu\n", info.stream);
        printf("offset = %hu\n", info.offset);
        printf("type = %hu\n", info.type);
        printf("method = %hu\n", info.method);
        printf("usage = %hu\n", info.usage);
        printf("usage_index = %hu\n", info.usage_index);

        if (info.type == LEVEL_VB_TYPE_UNUSED) {
            break;
        }

        if (count == capacity) {
            capacity *= 2;
            list = realloc(list, sizeof(LevelVBUsageInfo) * capacity);
        }
        list[count++] = info;
    }

    *out_count = count;
    return list;
}

// TEXTURE UV, usage_index == 0
static void read_texcoord_0(
        LevelVBReader * r,
        uint8_t type,
        bool has_corr,
        float correct_u,
        float correct_v,
        float * out_u,
        float * out_v
    ) {

    if (type == LEVEL_VB_TYPE_FLOAT2) {
        float u = vb_reader_f32(r);
        float v = vb_reader_f32(r);
        *out_u = u;
        *out_v = 1.0f - v;
        return;
    }

    if (type == LEVEL_VB_TYPE_SHORT2) {
        int16_t u = vb_reader_i16(r);
        int16_t v = vb_reader_i16(r);

        float uu = (float)u / LEVEL_VB_UV_COEFFICIENT;
        float vv = 1.0f - (float)v / LEVEL_VB_UV_COEFFICIENT;

        if (has_corr) {
            uu += correct_u * LEVEL_VB_UV_CORRECTOR_SCALE;
            vv -= correct_v * LEVEL_VB_UV_CORRECTOR_SCALE;
        }

        *out_u = uu;
        *out_v = vv;
        return;
    }

    if (type == LEVEL_VB_TYPE_SHORT4) {
        int16_t u = vb_reader_i16(r);
        int16_t v = vb_reader_i16(r);
        /* wind_coef */ vb_reader_u16(r);
        /* _ */         vb_reader_u16(r);

        float uu = (float)u / LEVEL_VB_UV_COEFFICIENT_2;
        float vv = 1.0f - (float)v / LEVEL_VB_UV_COEFFICIENT_2;

        if (has_corr) {
            uu += correct_u * LEVEL_VB_UV_CORRECTOR_SCALE;
            vv -= correct_v * LEVEL_VB_UV_CORRECTOR_SCALE;
        }

        *out_u = uu;
        *out_v = vv;
        return;
    }

    printf("unsupported TEXCOORD type: %u\n", type);
    exit(EXIT_FAILURE);
}

// LMAP UV, usage_index == 1
static void read_texcoord_1(
        LevelVBReader * r,
        uint8_t type,
        float * out_u,
        float * out_v
    ) {

    float u, v;

    if (type == LEVEL_VB_TYPE_FLOAT2) {
        u = vb_reader_f32(r);
        v = vb_reader_f32(r);

    } else if (type == LEVEL_VB_TYPE_SHORT2) {
        int16_t su = vb_reader_i16(r);
        int16_t sv = vb_reader_i16(r);
        u = (float)su / LEVEL_VB_LIGHT_MAP_COEFFICIENT;
        v = (float)sv / LEVEL_VB_LIGHT_MAP_COEFFICIENT;

    } else {
        printf("unsupported LMAP TEXCOORD type: %u\n", type);
        exit(EXIT_FAILURE);
    }

    *out_u = u;
    *out_v = 1.0f - v;
}

// Чтение одного поля вершины
static void read_vertex_field(
        LevelVBReader * r,
        const LevelVBUsageInfo * info,
        LevelVertexBuffer * vb,
        uint32_t vi,
        bool * has_corr,
        float * correct_u,
        float * correct_v
    ) {

    switch (info->usage) {

        case LEVEL_VB_USAGE_POSITION: {
            float x = vb_reader_f32(r);
            float y = vb_reader_f32(r);
            float z = vb_reader_f32(r);
            vb->positions[vi * 3 + 0] = x;
            vb->positions[vi * 3 + 1] = z;
            vb->positions[vi * 3 + 2] = y;
            break;
        }

        case LEVEL_VB_USAGE_NORMAL: {
            /* normal x */ vb_reader_u8(r);
            /* normal y */ vb_reader_u8(r);
            /* normal z */ vb_reader_u8(r);
            uint8_t hemi = vb_reader_u8(r);
            vb->color_hemi[vi] = hemi / 255.0f;
            break;
        }

        case LEVEL_VB_USAGE_TANGENT: {
            /* tan_x */ vb_reader_u8(r);
            /* tan_y */ vb_reader_u8(r);
            /* tan_z */ vb_reader_u8(r);
            *correct_u = vb_reader_u8(r);
            *has_corr  = true;
            break;
        }

        case LEVEL_VB_USAGE_BINORMAL: {
            /* bin_x */ vb_reader_u8(r);
            /* bin_y */ vb_reader_u8(r);
            /* bin_z */ vb_reader_u8(r);
            *correct_v = vb_reader_u8(r);
            *has_corr  = true;
            break;
        }

        case LEVEL_VB_USAGE_TEXCOORD: {
            if (info->usage_index == 0) {
                read_texcoord_0(
                    r, info->type, *has_corr, *correct_u, *correct_v,
                    &vb->uvs[vi * 2 + 0],
                    &vb->uvs[vi * 2 + 1]
                );
            } else if (info->usage_index == 1) {
                read_texcoord_1(
                    r, info->type,
                    &vb->uv_lmaps[vi * 2 + 0],
                    &vb->uv_lmaps[vi * 2 + 1]
                );
            } else {
                printf("unsupported TEXCOORD usage_index: %u\n", info->usage_index);
                exit(EXIT_FAILURE);
            }
            break;
        }

        case LEVEL_VB_USAGE_COLOR: {
            uint8_t blue  = vb_reader_u8(r);
            uint8_t green = vb_reader_u8(r);
            uint8_t red   = vb_reader_u8(r);
            uint8_t sun   = vb_reader_u8(r);
            vb->color_light[vi * 3 + 0] = red   / 255.0f;
            vb->color_light[vi * 3 + 1] = green / 255.0f;
            vb->color_light[vi * 3 + 2] = blue  / 255.0f;
            vb->color_sun[vi]           = sun   / 255.0f;
            break;
        }

        default:
            printf("unsupported vertex usage: %u\n", info->usage);
            exit(EXIT_FAILURE);
    }
}

// Выделение массивов под VB на основе declaration
static void level_vb_allocate(LevelVertexBuffer * vb, uint32_t count, const LevelVBUsageInfo * usages, uint32_t usage_count) {

    vb->count = count;

    for (uint32_t i = 0; i < usage_count; i++) {
        switch (usages[i].usage) {

            case LEVEL_VB_USAGE_POSITION:
                vb->positions   = malloc(sizeof(float) * 3 * count);
                vb->has_positions = true;
                break;

            case LEVEL_VB_USAGE_NORMAL:
                vb->normals    = malloc(sizeof(float) * 3 * count);
                vb->color_hemi = malloc(sizeof(float) * count);
                vb->has_normals = true;
                vb->has_hemi    = true;
                break;

            case LEVEL_VB_USAGE_TEXCOORD:
                if (usages[i].usage_index == 0) {
                    vb->uvs     = malloc(sizeof(float) * 2 * count);
                    vb->has_uvs = true;
                } else if (usages[i].usage_index == 1) {
                    vb->uv_lmaps     = malloc(sizeof(float) * 2 * count);
                    vb->has_uv_lmaps = true;
                }
                break;

            case LEVEL_VB_USAGE_COLOR:
                vb->color_light = malloc(sizeof(float) * 3 * count);
                vb->color_sun   = malloc(sizeof(float) * count);
                vb->has_colors  = true;
                vb->has_sun     = true;
                break;

            case LEVEL_VB_USAGE_TANGENT:
            case LEVEL_VB_USAGE_BINORMAL:
                vb->float_normals = true;
                break;
        }
    }
}

// Чтение одного VB
static void import_vertex_buffer_d3d9(LevelVBReader * r, LevelVertexBuffer * vb) {

    uint32_t usage_count = 0;
    LevelVBUsageInfo * usages = read_d3d9_declaration(r, &usage_count);

    uint32_t vertices_count = vb_reader_u32(r);

    level_vb_allocate(vb, vertices_count, usages, usage_count);

    for (uint32_t vi = 0; vi < vertices_count; vi++) {

        bool  has_corr  = false;
        float correct_u = 0.0f;
        float correct_v = 0.0f;

        for (uint32_t i = 0; i < usage_count; i++) {
            read_vertex_field(r, &usages[i], vb, vi, &has_corr, &correct_u, &correct_v);
        }
    }

    free(usages);
}

// Освобождение памяти
void free_level_vertex_buffer(LevelVertexBuffer * vb) {

    if (vb == NULL) {
        return;
    }

    free(vb->positions);
    free(vb->normals);
    free(vb->uvs);
    free(vb->uv_lmaps);
    free(vb->color_hemi);
    free(vb->color_light);
    free(vb->color_sun);

    memset(vb, 0, sizeof(*vb));
}

// Чтение буферов вершин
void read_level_vbs(LevelFile * level, FileData * geom_data, Chunks * chunks, uint32_t chunk_index) {

    (void) level;

    Chunk * chunk = &chunks->chunks[chunk_index];

    LevelVBReader r;
    vb_reader_init(&r, geom_data->bytes + chunk->offset, *chunk->size);

    uint32_t buffers_count = vb_reader_u32(&r);

    for (uint32_t i = 0; i < buffers_count; i++) {

        LevelVertexBuffer vb;
        memset(&vb, 0, sizeof(vb));

        import_vertex_buffer_d3d9(&r, &vb);

        printf(
            "VB[%u]: count=%u  pos=%d  norm=%d  uv=%d  lmap=%d  col=%d  hemi=%d  sun=%d\n",
            i, vb.count,
            vb.has_positions ? 1 : 0,
            vb.has_normals   ? 1 : 0,
            vb.has_uvs       ? 1 : 0,
            vb.has_uv_lmaps  ? 1 : 0,
            vb.has_colors    ? 1 : 0,
            vb.has_hemi      ? 1 : 0,
            vb.has_sun       ? 1 : 0
        );

        free_level_vertex_buffer(&vb);
    }

    if (r.pos != r.size) {
        printf("LEVEL_14_VB chunk: read %u of %u bytes\n", r.pos, r.size);
    }

}
