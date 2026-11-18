#ifndef IO_FUN_C
#define IO_FUN_C

#include "GLFW\glfw3.h"

#include "globals.c"
#include "overlay\overlay_grid.c"
#include "color_utils.c"

#include "app\app_loop.c"
#include "app\app_types.h"

static void get_camera_right_and_up(vec3 right, vec3 up) {
    float rad_x = glm_rad(camera_angle_x);
    float rad_y = glm_rad(-camera_angle_y);

    // Направление от центра к камере (радиус-вектор)
    vec3 to_camera = {
        cosf(rad_x) * sinf(rad_y),
        sinf(rad_x),
        cosf(rad_x) * cosf(rad_y)
    };

    // forward = от камеры к центру
    vec3 forward;
    glm_vec3_negate_to(to_camera, forward);
    glm_vec3_normalize(forward);

    // мировой up
    vec3 world_up = {0.0f, 1.0f, 0.0f};

    // right = world_up × forward
    glm_vec3_cross(world_up, forward, right);
    glm_vec3_normalize(right);

    // up = forward × right
    glm_vec3_cross(forward, right, up);
    glm_vec3_normalize(up);

}

void mouse_fun(GLFWwindow* window, int button, int action, int mods) {

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            is_rotating = 1;
            last_mouse_x = xpos;
            last_mouse_y = ypos;
        } else {
            is_rotating = 0;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS){
            is_panning = 1;
            last_pan_x = xpos;
            last_pan_y = ypos;
        } else {
            is_panning = 0;
        }
    }

}

void motion_fun(GLFWwindow* window, double xpos, double ypos) {

    if (is_rotating) {
        float delta_x = xpos - last_mouse_x;
        float delta_y = ypos - last_mouse_y;
        last_mouse_x = xpos;
        last_mouse_y = ypos;

        camera_angle_x += delta_y * 0.25f;
        camera_angle_y += delta_x * 0.25f;

        // Ограничиваем углы для удобства
        /*if (camera_angle_x > 89.0f) camera_angle_x = 89.0f;
        if (camera_angle_x < -89.0f) camera_angle_x = -89.0f;*/

    }

    if (is_panning) {
        float delta_x = (xpos - last_pan_x);
        float delta_y = (ypos - last_pan_y);

        last_pan_x = xpos;
        last_pan_y = ypos;

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
        glm_vec3_scale(up, delta_y * sensitivity, shift_up);

        // Общий сдвиг
        vec3 total_shift;
        glm_vec3_add(shift_right, shift_up, total_shift);

        // Применяем к pan_x, pan_y, pan_z
        pan_x += total_shift[0];
        pan_y += total_shift[1];
        pan_z += total_shift[2];

    }

}

void mouse_wheel(GLFWwindow* window, double xoffset, double yoffset) {
    // Увеличиваем или уменьшаем zoom
    if (yoffset > 0) {
        zoom /= 1.1f;
    } else if (yoffset < 0) {
        zoom *= 1.1f;
    }
}

// Обработка клавиатуры
void keyboard_fun(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Извлечь указатель на основную структуру программы
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    // Escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // W - рисовать рёбра
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        app->overlay_ctx.draw_edges = !app->overlay_ctx.draw_edges;

    // F - рисовать полигоны
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        app->overlay_ctx.draw_faces = !app->overlay_ctx.draw_faces;

    // P - рисовать точки
    } else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        app->overlay_ctx.draw_points = !app->overlay_ctx.draw_points;

    // G - рисовать координатную сетку
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        app->overlay_ctx.draw_grid = !app->overlay_ctx.draw_grid;

    // R - изменить рандомный цвет мешей
    } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        g_color_seed++;
        gen_random_meshes_color(app);

    // S - установить серый цвет мешей (Single Color)
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        gen_gray_meshes_color(app);

    // C - установить вид камеры по центру мешей
    } else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        set_camera_position(app);
    }

}

#endif    // IO_FUN_C
