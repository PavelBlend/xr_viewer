#ifndef OGF_VERTS_C
#define OGF_VERTS_C

#include <stdlib.h>

#include "types.h"

#define FVF_1L_SOC 0x12071980
#define FVF_2L_SOC 0x12071980 * 2

#define FVF_1L_CSCOP 1
#define FVF_2L_CSCOP 2
#define FVF_3L_CSCOP 3
#define FVF_4L_CSCOP 4

void read_verts(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {
    ogf_child->verts = malloc(sizeof(OgfVerts));

    uint32_t offset = chunk->offset;

    ogf_child->verts->format = *(uint32_t*)(file_data->bytes + offset);
    offset += sizeof(ogf_child->verts->format);

    ogf_child->verts->count = *(uint32_t*) (file_data->bytes + offset);
    offset += sizeof(ogf_child->verts->count);

    printf("      vertex format 0x%x\n", ogf_child->verts->format);
    // printf("      vertices count %d\n", ogf_child->verts->count);

    if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
        ogf_child->verts->verts_1_link = (OgfVert1Link*) (file_data->bytes + offset);

    } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
        ogf_child->verts->verts_2_link = (OgfVert2Link*) (file_data->bytes + offset);

    } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
        ogf_child->verts->verts_3_link = (OgfVert3Link*) (file_data->bytes + offset);

    } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
        ogf_child->verts->verts_4_link = (OgfVert4Link*) (file_data->bytes + offset);

    } else {
        printf("unsupported vertex format: 0x%x\n", ogf_child->verts->format);
        exit(EXIT_FAILURE);
    }

}

#endif    // OGF_VERTS_C
