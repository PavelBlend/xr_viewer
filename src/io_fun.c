#ifndef IO_FUN_C
#define IO_FUN_C

#include "globals.c"

static void get_camera_right_and_up(vec3 right, vec3 up) {
    float rad_x = glm_rad(camera_angle_x);
    float rad_y = glm_rad(-camera_angle_y);

    // Направление от центра к камере (радиус-вектор)
    vec3 to_camera = {
        cosf(rad_x) * sinf(rad_y),
        sinf(rad_x),
        cosf(rad_x) * cosf(rad_y)
    };

    // Forward = направление взгляда камеры (от камеры к центру) = -to_camera
    vec3 forward;
    glm_vec3_negate_to(to_camera, forward);

    // Up — как в вашем оригинальном коде (чтобы не переворачиваться при взгляде вниз)
    if (cosf(rad_x) < 0.0f) {
        up[0] = 0.0f; up[1] = -1.0f; up[2] = 0.0f;
    } else {
        up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
    }

    // Right = cross(up, forward)
    glm_vec3_cross(up, forward, right);
    glm_vec3_normalize(right);
}

void mouse_fun(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            is_rotating = 1;
            last_mouse_x = x;
            last_mouse_y = y;
        } else {
            is_rotating = 0;
        }
    }
    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            is_panning = 1;
            last_pan_x = x;
            last_pan_y = y;
        } else {
            is_panning = 0;
        }
    }
}

void motion_fun(int x, int y) {

    if (is_rotating) {
        float delta_x = x - last_mouse_x;
        float delta_y = y - last_mouse_y;
        last_mouse_x = x;
        last_mouse_y = y;

        camera_angle_x += delta_y * 0.25f;
        camera_angle_y += delta_x * 0.25f;

        // Ограничиваем углы для удобства
        /*if (camera_angle_x > 89.0f) camera_angle_x = 89.0f;
        if (camera_angle_x < -89.0f) camera_angle_x = -89.0f;*/

        // Обновляем экран после изменений
        glutPostRedisplay();
    }

    if (is_panning) {
        float delta_x = (x - last_pan_x);
        float delta_y = (y - last_pan_y);

        last_pan_x = x;
        last_pan_y = y;

        // Получаем локальные оси камеры
        vec3 right, up;
        get_camera_right_and_up(right, up);

        // Чувствительность зависит от zoom (чем дальше — тем медленнее панорамирование)
        float sensitivity = 0.002f * zoom;

        // Сдвиг по горизонтали (вправо/влево по экрану)
        vec3 shift_right;
        glm_vec3_scale(right, delta_x * sensitivity, shift_right);

        // Сдвиг по вертикали (вверх/вниз по экрану)
        vec3 shift_up;
        glm_vec3_scale(up, delta_y * sensitivity, shift_up);  // минус — потому что Y в окне вниз

        // Общий сдвиг
        vec3 total_shift;
        glm_vec3_add(shift_right, shift_up, total_shift);

        // Применяем к pan_x, pan_y, pan_z
        pan_x += total_shift[0];
        pan_y += total_shift[1];
        pan_z += total_shift[2];

        glutPostRedisplay();
    }

}

void mouse_wheel(int wheel, int direction, int x, int y) {
    // Увеличиваем или уменьшаем zoom
    if (direction > 0) {
        zoom /= 1.1f;
    } else if (direction < 0) {
        zoom *= 1.1f;
    }
    
    // Обновляем экран после изменений
    glutPostRedisplay();
}

void keyboard_fun(unsigned char key, int x, int y) {
    if (key == 27) {    // Esc
        glutLeaveMainLoop();
    }
}

#endif    // IO_FUN_C
