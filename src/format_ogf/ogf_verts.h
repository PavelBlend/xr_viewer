#pragma once

#include "..\types.h"
#include "..\chunk_io.h"

// ТЧ
#define FVF_1L_SOC 0x12071980
#define FVF_2L_SOC 0x12071980 * 2

// ЧН/ЗП
#define FVF_1L_CSCOP 1
#define FVF_2L_CSCOP 2
#define FVF_3L_CSCOP 3
#define FVF_4L_CSCOP 4

// Формат статичных вершин без скелета
#define FVF_OGF 0x112

void read_verts(OgfChild* ogf_child, FileData* file_data, Chunk* chunk);
