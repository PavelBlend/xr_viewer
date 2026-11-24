#ifndef LEVEL_SWIS_H
#define LEVEL_SWIS_H

#include <stdint.h>
#include "level_types.h"

// Один элемент скользящего окна (SlideWindowItem)
typedef struct LevelSWIItem {
    uint32_t offset;            // смещение в IB (в индексах)
    uint16_t triangles_count;
    uint16_t vertices_count;
} LevelSWIItem;

// Один SWI-буфер = массив LevelSWIItem
typedef struct LevelSWIBuffer {
    uint32_t        items_count;
    LevelSWIItem *  items;
} LevelSWIBuffer;

#endif    // LEVEL_SWIS_H
