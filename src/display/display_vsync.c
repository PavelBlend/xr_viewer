#ifndef DISPLAY_VSYNC_C
#define DISPLAY_VSYNC_C

void set_vsync(RenderContext * render_ctx) {
    if (render_ctx->vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

#endif    // DISPLAY_VSYNC_C
