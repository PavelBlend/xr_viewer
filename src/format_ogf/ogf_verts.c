#ifndef OGF_VERTS_C
#define OGF_VERTS_C

#include "ogf_verts.h"
#include "..\types.h"
#include "..\chunk_io.h"

// прочитать вершины ogf файла
void read_verts(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {

    // выделить память под структуру вершин
    ogf_child->verts = malloc(sizeof(OgfVerts));

    // текущее смещение в файле
    uint32_t offset = chunk->offset;

    // прочитать формат вершин
    ogf_child->verts->format = *(uint32_t*)(file_data->bytes + offset);

    // пропустить формат вершин
    offset += sizeof(ogf_child->verts->format);

    // прочитать количество вершин
    ogf_child->verts->count = *(uint32_t*) (file_data->bytes + offset);

    // пропустить количество вершин
    offset += sizeof(ogf_child->verts->count);

    // прочитать вершины

    // 1-link вершины
    if (ogf_child->verts->format == FVF_1L_SOC || ogf_child->verts->format == FVF_1L_CSCOP) {
        ogf_child->verts->verts_1_link = (OgfVert1Link*) (file_data->bytes + offset);

    // 2-link вершины
    } else if (ogf_child->verts->format == FVF_2L_SOC || ogf_child->verts->format == FVF_2L_CSCOP) {
        ogf_child->verts->verts_2_link = (OgfVert2Link*) (file_data->bytes + offset);

    // 3-link вершины
    } else if (ogf_child->verts->format == FVF_3L_CSCOP) {
        ogf_child->verts->verts_3_link = (OgfVert3Link*) (file_data->bytes + offset);

    // 4-link вершины
    } else if (ogf_child->verts->format == FVF_4L_CSCOP) {
        ogf_child->verts->verts_4_link = (OgfVert4Link*) (file_data->bytes + offset);

    // статичные вершины без скелета
    } else if (ogf_child->verts->format == FVF_OGF) {
        ogf_child->verts->verts_static = (OgfVertStatic*) (file_data->bytes + offset);

    // неподдерживаемый формат вершин
    } else {
        printf("unsupported vertex format: 0x%x\n", ogf_child->verts->format);
        exit(EXIT_FAILURE);
    }

}

#endif    // OGF_VERTS_C
