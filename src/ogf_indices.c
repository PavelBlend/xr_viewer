#ifndef OGF_INDICES_C
#define OGF_INDICES_C

#include <inttypes.h>

#include "types.h"
#include "chunk_io.h"

// прочитать индексы треугольников ogf файла
void read_indices(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {

    // текущее смещение в файле
    uint32_t offset = chunk->offset;

    // выделить память под структуру индексов
    ogf_child->indices = malloc(sizeof(OgfIndices));

    // прочитать количество индексов
    ogf_child->indices->count = *(uint32_t*) (file_data->bytes + offset);

    // пропустить количество индексов
    offset += sizeof(ogf_child->indices->count);

    // прочитать индексы
    ogf_child->indices->indices = (uint16_t*) (file_data->bytes + offset);

}

#endif    // OGF_INDICES_C
