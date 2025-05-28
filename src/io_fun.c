#ifndef IO_FUN_C
#define IO_FUN_C

#include "globals.c"

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
        float delta_x = ((x - last_pan_x) / zoom) * 0.0025f;  // Коэффициент для чувствительности
        float delta_y = ((y - last_pan_y) / zoom) * 0.0025f;
        pan_x += delta_x;
        pan_y -= delta_y;
        last_pan_x = x;
        last_pan_y = y;

        glutPostRedisplay();
    }
}

void mouse_wheel(int wheel, int direction, int x, int y) {
    // Увеличиваем или уменьшаем zoom
    if (direction > 0) {
        zoom *= 1.1f; // При прокрутке вверх увеличиваем масштаб
    } else if (direction < 0) {
        zoom /= 1.1f; // При прокрутке вниз уменьшаем масштаб
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
