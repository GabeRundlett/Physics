#include <cmath>
#include <functional>
#include <string>
#include <array>

#include "Scene.hpp"

int main() {
    glfwInit();
    window = glfwCreateWindow(window_width, window_height, "Physics", nullptr, nullptr);
    drag_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    select_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    if (!window) {
        std::printf("Failed to create window");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetCursorPosCallback(window, mouse_move);
    glfwSetMouseButtonCallback(window, mouse_press);
    glfwSetWindowSizeCallback(window, window_resize);
    glfwSetKeyCallback(window, key_press);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

#if 0
    snap_to_grid = false;
    for (auto j = 0.0f; j < 100; ++j) {
        for (auto i = 0.0f; i < 100; ++i) {
            mouse = {20.0f * i, 10.0f * j};
            set_line_point1();
            mouse = {20.0f * i + 10, 10.0f * j};
            set_line_point2();
        }
    }
#endif

    begin();

    int frame_num = 0;
    double last_frame_time = 0;

    while (!glfwWindowShouldClose(window)) {
        prev_time = current_time;
        current_time = glfwGetTime();
        elapsed_time = current_time - prev_time;

        ++frame_num;
        if (frame_num > 1000) {
            avg_time = 1.0 / 100 * (current_time - last_frame_time);
            last_frame_time = current_time;
            frame_num = 0;
        }

        draw_scene();
        flush();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}
