#ifndef OGF_INDICES_C
#define OGF_INDICES_C

#include "types.h"

void read_indices(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {
    ogf_child->indices = malloc(sizeof(OgfIndices));

    uint32_t offset = chunk->offset;

    ogf_child->indices->count = (uint32_t*) (file_data->bytes + offset);
    offset += sizeof(*ogf_child->indices->count);

    ogf_child->indices->indices = (uint16_t*) (file_data->bytes + offset);
}

#endif    // OGF_INDICES_C
