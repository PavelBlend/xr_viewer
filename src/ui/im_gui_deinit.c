#pragma once

// Деинициализация CImGUI
void c_im_gui_deinit(void) {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

}
