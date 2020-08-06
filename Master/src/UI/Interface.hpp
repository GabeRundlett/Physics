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
static unsigned int tool_id = 1, prev_tool_id = 1;

static inline vec2 &get_grid_mouse() {
    if (snap_to_grid)
        return grid_mouse;
    return mouse;
}

#include "UI/Line.hpp"
#include "UI/Rect.hpp"
#include "UI/Button.hpp"

static inline void set_cam_pos(const vec2 &v) {
    cam_pos = v;
    glUniform2fv(glGetUniformLocation(s_shader, "cam_pos"), 1, (const float *)&cam_pos);
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

static void window_resize(GLFWwindow *, int w, int h) {
    window_width = w, window_height = h;
    glViewport(0, 0, w, h);
    glUniform2f(glGetUniformLocation(s_shader, "window_size"), static_cast<float>(w), static_cast<float>(h));
}
static void mouse_move(GLFWwindow *, double x, double y) {
    mouse.x = static_cast<float>(x), mouse.y = window_height - static_cast<float>(y);
    grid_mouse = {10.f * int((mouse.x - cam_pos.x) / 10 + 0.5f) + cam_pos.x,
                  10.f * int((mouse.y - cam_pos.y) / 10 + 0.5f) + cam_pos.y};

    if (tool_id == 1) {
        hover_lines();
        hover_rects();
    }

    drag_line_handle();
    drag_line();

    if (place_mode) {
        switch (tool_id) {
        case 0: break;
        case 1: break;
        case 2: drag_line_point2(); break;
        default: break;
        }
    }

    drag_camera();
}
static void mouse_press(GLFWwindow *, int button, int action, int) {
    switch (action) {
    case GLFW_PRESS:
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            if (mouse.x > 200) {
                switch (tool_id) {
                case 0: break;
                case 1:
                    grab_line_handle();
                    grab_line();
                    break;
                case 2: set_line_point1(); break;
                default: break;
                }
            }
            break;
        case GLFW_MOUSE_BUTTON_3: grab_camera(); break;
        default: break;
        }
        break;
    case GLFW_RELEASE:
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            if (tool_id == 1) {
                place_line_handle();
                place_line();
            }
            if (place_mode) {
                switch (tool_id) {
                case 0: break;
                case 1: break;
                case 2: set_line_point2(); break;
                default: break;
                }
            }
            break;
        case GLFW_MOUSE_BUTTON_3: place_camera(); break;
        default: break;
        }
        break;
    default: break;
    }
}
static void key_press(GLFWwindow *glfw_window, int key, int, int action, int) {
    switch (key) {
    case GLFW_KEY_D:
        switch (action) {
        case GLFW_PRESS:
            glfwSetCursor(glfw_window, select_cursor);
            tool_id = 2;
            break;
        default: break;
        }
        break;
    case GLFW_KEY_R: set_cam_pos({0, 0}); break;
    case GLFW_KEY_LEFT_CONTROL:
        switch (action) {
        case GLFW_PRESS: snap_to_grid = true; break;
        case GLFW_RELEASE: snap_to_grid = false; break;
        default: break;
        }
        break;
    default: break;
    }
}
