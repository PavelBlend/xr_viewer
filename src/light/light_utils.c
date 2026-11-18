#ifndef LIGHT_UTILS_C
#define LIGHT_UTILS_C

void add_light(GLFWwindow* window, RenderContext * render_ctx) {

    // направление света в пространстве вида (следует за камерой),
    // переведённое обратно в мир через инверсию view

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    float rad_x = glm_rad(camera_angle_x);
    float rad_y = glm_rad(-camera_angle_y);

    vec3 up = {0.0f, 1.0f, 0.0f};
    if (cosf(rad_x) < 0.0f) {
        up[1] = -1.0f;
    }

    vec3 center = {pan_x, pan_y, pan_z};
    vec3 eye = {
        pan_x + zoom * cosf(rad_x) * sinf(rad_y),
        pan_y + zoom * sinf(rad_x),
        pan_z + zoom * cosf(rad_x) * cosf(rad_y)
    };

    mat4 view;
    glm_lookat(eye, center, up, view);

    // матрица вида 3x3
    mat3 view3;
    glm_mat4_pick3(view, view3);

    float lx = 0.0f;
    float ly = 0.0f;
    float lz = 1.0f;

    // направление света в пространстве вида (константа)
    vec3 light_view = {lx, ly, lz};
    glm_vec3_normalize(light_view);

    // перевод в мир: world = transpose(view3) * view
    vec3 light_world;
    glm_mat3_mulv(view3, light_view, light_world); // если cglm требует — используйте transpose
    glm_vec3_normalize(light_world);

    mat3 view3t;
    glm_mat3_transpose_to(view3, view3t);
    glm_mat3_mulv(view3t, light_view, light_world);

    glUniform3f(render_ctx->lightDirLoc, light_world[0], light_world[1], light_world[2]);

}

#endif    // LIGHT_UTILS_C
