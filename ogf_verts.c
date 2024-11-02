#ifndef OGF_VERTS_C
#define OGF_VERTS_C

#include <stdlib.h>

#include "types.h"

#define FVF_1L_SOC 0x12071980
#define FVF_2L_SOC 0x12071980 * 2

void read_verts(OgfFile* ogf_file, FileData* file_data, Chunk* chunk) {
    ogf_file->verts = malloc(sizeof(OgfVerts));

    uint32_t offset = chunk->offset;

    ogf_file->verts->format = (uint32_t*) (file_data->bytes + offset);
    offset += sizeof(*ogf_file->verts->format);

    ogf_file->verts->count = (uint32_t*) (file_data->bytes + offset);
    offset += sizeof(*ogf_file->verts->count);

    // printf("      vertex format 0x%x\n", *ogf_file->verts->format);
    // printf("      vertices count %d\n", *ogf_file->verts->count);

    if (*ogf_file->verts->format == FVF_1L_SOC) {
        ogf_file->verts->verts_1_link = (OgfVert1Link*) (file_data->bytes + offset);

    } else if (*ogf_file->verts->format == FVF_2L_SOC) {
        ogf_file->verts->verts_2_link = (OgfVert2Link*) (file_data->bytes + offset);

    } else {
        printf("unsupported vertex format: 0x%x\n", *ogf_file->verts->format);
        exit(EXIT_FAILURE);
    }

}

#endif    // OGF_VERTS_C
