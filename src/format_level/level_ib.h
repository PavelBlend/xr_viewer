#ifndef LEVEL_IB_H
#define LEVEL_IB_H

#include <stdint.h>
#include "level_types.h"

typedef struct LevelIndexBuffer {
    uint32_t   count;       // количество индексов
    uint16_t * indices;     // массив индексов (count штук)
} LevelIndexBuffer;

// Прочитать чанк LEVEL_14_IB
void read_level_ibs(LevelFile * level, FileData * geom_data, Chunks * chunks, uint32_t chunk_index);

// Освободить один IB
void free_level_index_buffer(LevelIndexBuffer * ib);

#endif    // LEVEL_IB_H
