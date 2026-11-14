#ifndef OGF_VERTS_H
#define OGF_VERTS_H

#include "types.h"
#include "chunk_io.h"

#define FVF_1L_SOC 0x12071980
#define FVF_2L_SOC 0x12071980 * 2

#define FVF_1L_CSCOP 1
#define FVF_2L_CSCOP 2
#define FVF_3L_CSCOP 3
#define FVF_4L_CSCOP 4

#define FVF_OGF 0x112

void read_verts(OgfChild* ogf_child, FileData* file_data, Chunk* chunk);

#endif    // OGF_VERTS_H
