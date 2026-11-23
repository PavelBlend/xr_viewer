#ifndef OGF_TEX_C
#define OGF_TEX_C

#include "..\types.h"
#include "..\string.c"

#include "..\chunk_io.h"

// прочитать текстуру ogf файла
void read_texture(OgfChild* ogf_child, Application * app, Chunk* chunk) {

    // прочитать относительный путь к текстуре
    ogf_child->texture = read_string_zero(app->file_data, &chunk->offset);

    // длина пути к textures папке
    uint32_t tex_folder_length  = get_string_length(app->prefs.tex_folder);

    // длина относительного пути к текстуре
    uint32_t tex_relpath_length = get_string_length(ogf_child->texture);

    // длина расширения .dds
    uint32_t tex_ext_length     = get_string_length(app->prefs.tex_ext);

    // длина всего абсолютного пути (+1 в конце - нулевой байт)
    uint32_t tex_abspath_length = tex_folder_length + tex_relpath_length + tex_ext_length + 1;

    // выделить память под абсолютный путь к текстуре
    ogf_child->texture_path = malloc(tex_abspath_length);

    // записать символы пути к textures папке
    for (uint32_t i=0; i<tex_folder_length; i++) {
        ogf_child->texture_path[i] = app->prefs.tex_folder[i];
    }

    // записать символы относительного пути к текстуре
    for (uint32_t i=0; i<tex_relpath_length; i++) {
        ogf_child->texture_path[tex_folder_length + i] = ogf_child->texture[i];
    }

    // записать символы расширения
    for (uint32_t i=0; i<tex_ext_length; i++) {
        ogf_child->texture_path[tex_folder_length + tex_relpath_length + i] = app->prefs.tex_ext[i];
    }

    // записать нулевой байт
    ogf_child->texture_path[tex_abspath_length - 1] = 0;

}

#endif    // OGF_TEX_C
