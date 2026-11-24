#ifndef OGF_TEX_C
#define OGF_TEX_C

#include "..\types.h"
#include "..\string.c"
#include "..\tex_io.c"

#include "..\chunk_io.h"

// прочитать текстуру ogf файла
void read_texture(OgfChild* ogf_child, Application * app, Chunk* chunk) {

    // прочитать относительный путь к текстуре
    ogf_child->texture = read_string_zero(app->file_data, &chunk->offset);

    // вычислить абсолютный путь к текстуре
    ogf_child->texture_path = get_abs_tex_path(app, ogf_child->texture);

}

#endif    // OGF_TEX_C
