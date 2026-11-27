#pragma once

// Деинициализировать программу
void deinit_app(Application * app) {

    // Очистка OpenGL данных
    clear_gl_data(&app->render_ctx);

    // Деинициализировать CImGUI
    c_im_gui_deinit();

    // Деинициализировать GLFW
    glfw_deinit(app);

}
