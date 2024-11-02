#ifndef OGF_INDICES_C
#define OGF_INDICES_C

#include "types.h"

void read_indices(OgfFile* ogf_file, FileData* file_data, Chunk* chunk) {
    ogf_file->indices = malloc(sizeof(OgfIndices));

    uint32_t offset = chunk->offset;

    ogf_file->indices->count = (uint32_t*) (file_data->bytes + offset);
    offset += sizeof(*ogf_file->indices->count);

    ogf_file->indices->indices = (uint16_t*) (file_data->bytes + offset);
}

#endif    // OGF_INDICES_C
