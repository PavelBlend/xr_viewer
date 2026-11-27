#pragma once

#include <stdint.h>
#include "level_types.h"

typedef struct LevelIndexBuffer {
    uint32_t   count;       // количество индексов
    uint16_t * indices;     // массив индексов (count штук)
} LevelIndexBuffer;
