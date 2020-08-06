#pragma once
#include <GLFW/glfw3.h>
#include "../Renderer/Renderer.hpp"

static GLFWwindow *window;
static GLFWcursor *drag_cursor, *select_cursor;

static vec2 mouse = {0, 0};
static vec2 grid_mouse = {0, 0};
static vec2 cam_pos = {0, 0};
static int window_width = 1600, window_height = 900;
static double current_time = 0, prev_time = 0, elapsed_time = 1, avg_time = 1;

static constexpr float menu_width = 200;
static constexpr vec2 button_margin = {40, 6};

static vec2 temp_cam_pos = {0, 0};
static vec2 viewport_center = {
    (static_cast<float>(window_width) - menu_width) / 2 + menu_width,
    static_cast<float>(window_height) / 2,
};
static bool cam_move = false;
static bool place_mode = false, snap_to_grid = false;
static unsigned int tool_id = 1;

static inline vec2 &get_grid_mouse() {
    if (snap_to_grid)
        return grid_mouse;
    return mouse;
}
static inline void set_cam_pos(const vec2 &v) {
    cam_pos = v;
    glUniform2fv(glGetUniformLocation(s_shader, "cam_pos"), 1, reinterpret_cast<const float *>(&cam_pos));
}
static inline void drag_camera() {
    if (cam_move) {
        set_cam_pos(mouse - temp_cam_pos);
        glfwSetCursor(window, drag_cursor);
    }
}
static inline void grab_camera() {
    glfwSetCursor(window, drag_cursor);
    temp_cam_pos = mouse - cam_pos;
    cam_move = true;
}
static inline void place_camera() {
    cam_pos = mouse - temp_cam_pos;
    cam_move = false;
    glfwSetCursor(window, nullptr);
}
