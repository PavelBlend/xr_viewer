#ifndef OGF_TEX_C
#define OGF_TEX_C

#include "types.h"
#include "string.c"
#include "textures.c"

void read_texture(OgfChild* ogf_child, FileData* file_data, Chunk* chunk) {
    ogf_child->texture = read_string_zero(file_data, &chunk->offset);

    // get absolute texture path

    // calculate absolute path length
    uint32_t tex_folder_length = get_string_length(tex_folder);
    uint32_t tex_relpath_length = get_string_length(ogf_child->texture);
    uint32_t tex_ext_length = get_string_length(tex_ext);
    uint32_t tex_abspath_length = tex_folder_length + tex_relpath_length + tex_ext_length + 1;

    // create absolute path string
    ogf_child->texture_path = malloc(tex_abspath_length);

    for (uint32_t i=0; i<tex_folder_length; i++) {
        ogf_child->texture_path[i] = tex_folder[i];
    }

    for (uint32_t i=0; i<tex_relpath_length; i++) {
        ogf_child->texture_path[tex_folder_length + i] = ogf_child->texture[i];
    }

    for (uint32_t i=0; i<tex_ext_length; i++) {
        ogf_child->texture_path[tex_folder_length + tex_relpath_length + i] = tex_ext[i];
    }

    ogf_child->texture_path[tex_abspath_length - 1] = 0;

}

#endif    // OGF_TEX_C
