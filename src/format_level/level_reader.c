#ifndef LEVEL_READER_C
#define LEVEL_READER_C

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LevelReader {
    const uint8_t * data;
    uint32_t        size;
    uint32_t        pos;
} LevelReader;

static void level_reader_init(LevelReader * r, const uint8_t * data, uint32_t size) {
    r->data = data;
    r->size = size;
    r->pos  = 0;
}

static void level_reader_require(LevelReader * r, uint32_t bytes) {
    if (r->pos + bytes > r->size) {
        printf("level reader out of bounds: pos=%u, need=%u, size=%u\n",
               r->pos, bytes, r->size);
        exit(EXIT_FAILURE);
    }
}

static uint8_t level_reader_u8(LevelReader * r) {
    level_reader_require(r, 1);
    return r->data[r->pos++];
}

static uint16_t level_reader_u16(LevelReader * r) {
    level_reader_require(r, 2);
    uint16_t v;
    memcpy(&v, r->data + r->pos, 2);
    r->pos += 2;
    return v;
}

static int16_t level_reader_i16(LevelReader * r) {
    level_reader_require(r, 2);
    int16_t v;
    memcpy(&v, r->data + r->pos, 2);
    r->pos += 2;
    return v;
}

static uint32_t level_reader_u32(LevelReader * r) {
    level_reader_require(r, 4);
    uint32_t v;
    memcpy(&v, r->data + r->pos, 4);
    r->pos += 4;
    return v;
}

static float level_reader_f32(LevelReader * r) {
    level_reader_require(r, 4);
    float v;
    memcpy(&v, r->data + r->pos, 4);
    r->pos += 4;
    return v;
}

#endif    // LEVEL_READER_C
