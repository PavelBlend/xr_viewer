#ifndef IM_GUI_DEINIT_C
#define IM_GUI_DEINIT_C

// Деинициализация CImGUI
void c_im_gui_deinit(void) {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

}

#endif    // IM_GUI_DEINIT_C
