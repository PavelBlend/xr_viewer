#ifndef TYPES_H
#define TYPES_H

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

typedef struct OgfVerts{
    uint32_t* format;
    uint32_t* count;
    OgfVert1Link* verts_1_link;
    OgfVert2Link* verts_2_link;
} OgfVerts;

typedef struct OgfIndices {
    uint32_t* count;
    uint16_t* indices;
} OgfIndices;

typedef struct OgfChild {
    OgfVerts* verts;
    OgfIndices* indices;
} OgfChild;

typedef struct OgfFile {
    uint32_t children_count;
    OgfChild* children;
} OgfFile;

#endif    // TYPES_H
