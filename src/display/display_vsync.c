#pragma once

void set_vsync(RenderContext * render_ctx) {
    if (render_ctx->vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}
