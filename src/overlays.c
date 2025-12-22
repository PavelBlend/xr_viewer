#ifndef OVERLAYS_C
#define OVERLAYS_C

void render_navigation_center() {

    static GLuint axis_vao = 0, axis_vbo = 0;
    static int axis_initialized = 0;

    if (!axis_initialized) {
        glGenVertexArrays(1, &axis_vao);
        glGenBuffers(1, &axis_vbo);
        axis_initialized = 1;
    }

    // 6 вершин: по две на каждую ось (±0.1 по оси)
    float axis_scale = 0.05f; // размер креста в мировых единицах
    float lines[18] = {
        pan_x - axis_scale, pan_y, pan_z,   pan_x + axis_scale, pan_y, pan_z,   // X
        pan_x, pan_y - axis_scale, pan_z,   pan_x, pan_y + axis_scale, pan_z,   // Y
        pan_x, pan_y, pan_z - axis_scale,   pan_x, pan_y, pan_z + axis_scale    // Z
    };

    glBindVertexArray(axis_vao);
    glBindBuffer(GL_ARRAY_BUFFER, axis_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0f);
}

void render_overlays() {
    glBindTexture(GL_TEXTURE_2D, 0);
    render_navigation_center();
}

#endif    // OVERLAYS_C
