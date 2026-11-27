#pragma once

void generate_bbox(Application * app) {

    // размер вершин bbox (8 вершин параллелепипеда)
    size_t bbox_verts_size = sizeof(float) * 3 * 8;

    for (uint32_t i=0; i<app->overlay_ctx.bbox_children_count; i++) {

        // Выделяем память для вершин bbox
        float* bbox_vertices = malloc(bbox_verts_size);

        OgfFile* ogf_file = (OgfFile*) app->file;

        // Генерация координат вершин bbox
        for (int vert_id=0; vert_id<8; ++vert_id) {
            for (int axis=0; axis<3; ++axis) {
                bbox_vertices[vert_id*3 + axis] = (vert_id & (1 << axis)) ? *ogf_file->children[i].header.bbox_max[axis] : *ogf_file->children[i].header.bbox_min[axis];
            }
        }

        // Создать вершинный буфер для bbox
        glGenVertexArrays(1, &app->overlay_ctx.bbox_vao[i]);
        glBindVertexArray(app->overlay_ctx.bbox_vao[i]);

        glGenBuffers(1, &app->overlay_ctx.bbox_vertex_buffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, app->overlay_ctx.bbox_vertex_buffers[i]);

        glBufferData(GL_ARRAY_BUFFER, bbox_verts_size, bbox_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

        // Настройка атрибута позиции
        glEnableVertexAttribArray(0);

        // Создать индексный буфер для bbox

        // 12 рёбер куба
        uint32_t bbox_indices_count = 12 * 2;
        size_t bbox_indices_size = sizeof(uint16_t) * (size_t)bbox_indices_count;

        uint16_t* bbox_indices = malloc(bbox_indices_size);

        // генерировать индексы рёбер bbox (массив из 24 uint32, 12 рёбер × 2)
        int idx = 0;
        for (int vert_id=0; vert_id<8; ++vert_id) {
            for (int axis=0; axis<3; ++axis) {
                int neighbor = vert_id ^ (1 << axis);
                if (neighbor > vert_id) {
                    bbox_indices[idx++] = vert_id;
                    bbox_indices[idx++] = neighbor;
                }
            }
        }

        glGenBuffers(1, &app->overlay_ctx.bbox_index_buffers[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->overlay_ctx.bbox_index_buffers[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bbox_indices_size, bbox_indices, GL_STATIC_DRAW);

        app->overlay_ctx.bbox_indices_count[i] = bbox_indices_count;

    }

}
