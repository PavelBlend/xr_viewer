#ifndef COLOR_UTILS_C
#define COLOR_UTILS_C

#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

// seed для генератора цвета
static uint32_t g_color_seed = 0;

void init_color_seed(void) {
    g_color_seed = (uint32_t)time(NULL);
}

// хеш-функция для генерации псевдослучайного цвета по id
static void random_color_from_id(uint32_t id, float* r, float* g, float* b) {
    // Перемешиваем биты, чтобы соседние id давали разные цвета
    uint32_t h = (id + 1) * 2654435761u + g_color_seed; // множитель Кнута
    h ^= h >> 16;
    h *= 0x85ebca6bu;
    h ^= h >> 13;
    h *= 0xc2b2ae35u;
    h ^= h >> 16;

    // Извлекаем три байта и нормируем в [0, 1]
    *r = ((h >>  0) & 0xFF) / 255.0f;
    *g = ((h >>  8) & 0xFF) / 255.0f;
    *b = ((h >> 16) & 0xFF) / 255.0f;
}

#endif    // COLOR_UTILS_C
