#ifndef TYPES_H
#define TYPES_H

#pragma pack(push, 1)

typedef struct OgfVert1Link{

    float pos_x;
    float pos_y;
    float pos_z;

    float nor_x;
    float nor_y;
    float nor_z;

    float tan_x;
    float tan_y;
    float tan_z;

    float bit_x;
    float bit_y;
    float bit_z;

    float tex_u;
    float tex_v;

    uint32_t bone;

} OgfVert1Link;

typedef struct OgfVert2Link{

    uint16_t bone_1;
    uint16_t bone_2;

    float pos_x;
    float pos_y;
    float pos_z;

    float nor_x;
    float nor_y;
    float nor_z;

    float tan_x;
    float tan_y;
    float tan_z;

    float bit_x;
    float bit_y;
    float bit_z;

    float weight;

    float tex_u;
    float tex_v;

} OgfVert2Link;

typedef struct OgfVert3Link{

    uint16_t bone_1;
    uint16_t bone_2;
    uint16_t bone_3;

    float pos_x;
    float pos_y;
    float pos_z;

    float nor_x;
    float nor_y;
    float nor_z;

    float tan_x;
    float tan_y;
    float tan_z;

    float bit_x;
    float bit_y;
    float bit_z;

    float weight_1;
    float weight_2;

    float tex_u;
    float tex_v;

} OgfVert3Link;

typedef struct OgfVert4Link{

    uint16_t bone_1;
    uint16_t bone_2;
    uint16_t bone_3;
    uint16_t bone_4;

    float pos_x;
    float pos_y;
    float pos_z;

    float nor_x;
    float nor_y;
    float nor_z;

    float tan_x;
    float tan_y;
    float tan_z;

    float bit_x;
    float bit_y;
    float bit_z;

    float weight_1;
    float weight_2;
    float weight_3;

    float tex_u;
    float tex_v;

} OgfVert4Link;

typedef struct OgfVertStatic{

    float pos_x;
    float pos_y;
    float pos_z;

    float nor_x;
    float nor_y;
    float nor_z;

    float tex_u;
    float tex_v;

} OgfVertStatic;

typedef struct OgfVerts{
    uint32_t format;
    uint32_t count;
    OgfVert1Link* verts_1_link;
    OgfVert2Link* verts_2_link;
    OgfVert3Link* verts_3_link;
    OgfVert4Link* verts_4_link;
    OgfVertStatic* verts_static;
} OgfVerts;

typedef struct OgfIndices {
    uint32_t count;
    uint16_t* indices;
} OgfIndices;

typedef struct OgfSwis {
    uint32_t tris_offset;
    uint16_t tris_count;
    uint16_t verts_count;
} OgfSwis;

typedef struct OgfChild {
    char*           texture;
    char*           texture_path;
    OgfVerts*       verts;
    OgfIndices*     indices;
    OgfSwis*        swis;
} OgfChild;

typedef struct OgfFile {
    uint32_t children_count;
    OgfChild* children;
} OgfFile;

#endif    // TYPES_H
