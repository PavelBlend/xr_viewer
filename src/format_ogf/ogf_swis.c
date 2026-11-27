#pragma once

#include "..\types.h"

void read_swis(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {
    ogf_child->swis = malloc(sizeof(OgfSwis));

    uint32_t offset = chunk->offset;

    offset += sizeof(uint32_t) * 4;    // reserved

    uint32_t* swis_count = (uint32_t*) (file_data->bytes + offset);
    offset += sizeof(uint32_t);

    for (uint32_t swi_id=0; swi_id<*swis_count; swi_id++) {

        ogf_child->swis->tris_offset = *(uint32_t*) (file_data->bytes + offset);
        offset += sizeof(ogf_child->swis->tris_offset);

        ogf_child->swis->tris_count = *(uint16_t*) (file_data->bytes + offset);
        offset += sizeof(ogf_child->swis->tris_count);

        ogf_child->swis->verts_count = *(uint16_t*) (file_data->bytes + offset);
        offset += sizeof(ogf_child->swis->verts_count);

        // Прочитать самый первый SWI и выйти из цикла
        break;

    }

    ogf_child->indices->indices += ogf_child->swis->tris_offset;
    ogf_child->indices->count = ogf_child->swis->tris_count * 3;

}
