#ifndef LEVEL_HEADER_h
#define LEVEL_HEADER_h

#include "level_types.h"

void read_level_header(LevelFile * level, LevelHeader * header, uint8_t * file_bytes, Chunks * chunks, uint32_t chunk_index, char * file_path, bool is_level_geom_file);

#endif    // LEVEL_HEADER_h
