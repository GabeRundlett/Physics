#include <cmath>
#include <debug.hpp>
#include <functional>
#include <string>
#include <vector>

#include "renderer.hpp"
#include <GLFW/glfw3.h>

struct Line {
    vec2 a, b;
};
struct Rect {
    vec2 pos, size;
};

static vec2 mouse = {0, 0};
static vec2 grid_mouse = {0, 0};
static vec2 cam_pos = {0, 0};
static float window_width = 1600, window_height = 900;

static constexpr float menu_width = 200;
constexpr vec2 button_margin = {40, 6};

static vec2 temp_cam_pos = {0, 0};
static vec2 viewport_center = {(window_width - menu_width) / 2 + menu_width, window_height / 2};
static bool cam_move = false;

static std::vector<Line> lines;
static std::vector<Rect> rects;
static bool place_mode = false;
static unsigned int tool_id = 1, prev_tool_id = 1;

static bool grabbed_line = false, grabbed_line_handle = false, snap_to_grid = false;
static vec2 *hovered_line_handle = nullptr, grabbed_line_offset_a, grabbed_line_offset_b;
static Line *hovered_line = nullptr;

static bool grabbed_rect_handle = false;
static unsigned char rect_handle_id = 0;
static Rect *hovered_rect = nullptr;

GLFWwindow *window;
GLFWcursor *drag_cursor, *select_cursor;

// #define LIGHT_THEME

#ifndef LIGHT_THEME
static color menu_color = {25, 25, 28, 255}, menu_border_color = {20, 20, 22, 255},  //
    text_color1 = {168, 168, 172, 255}, drop_shadow_color1 = {0, 0, 0, 75},          //
    button_color1 = {22, 22, 24, 255}, button_color2 = {30, 30, 33, 255},            //
    selection_color1 = {100, 100, 200, 255}, selection_color2 = {200, 200, 255, 25}, //
    line_color = {61, 61, 66, 255}, handle_color = {82, 82, 91, 255};
#else
static color menu_color = {255, 255, 255, 255}, menu_border_color = {200, 200, 200, 255}, //
    text_color1 = {50, 50, 50, 255}, drop_shadow_color1 = {255, 255, 255, 75},            //
    button_color1 = {210, 210, 210, 255}, button_color2 = {30, 30, 33, 255},              //
    selection_color1 = {100, 100, 200, 255},                                              //
    line_color = {180, 180, 180, 255}, handle_color = {150, 150, 150, 255};
#endif

static constexpr inline vec2 get_button_pos(const float length, const float height) {
    return {menu_width / 2 - length * 4 - button_margin.x, window_height - height - button_margin.y};
}
static constexpr inline vec2 get_button_size(const float length) {
    return {length * 8 + button_margin.x * 2, 16 + button_margin.y * 2};
}

static inline void set_cam_pos(const vec2 &v) {
    cam_pos = v;
    glUniform2fv(glGetUniformLocation(s_shader, "cam_pos"), 1, (const float *)&cam_pos);
}

static inline vec2 &get_grid_mouse() {
    if (snap_to_grid) return grid_mouse;
    return mouse;
}

static inline void hover_lines() {
    if (lines.size())
        if (!grabbed_line && !grabbed_line_handle) {
            for (Line &l : lines) {
                const vec2 m = mouse - cam_pos;
                constexpr float radius = 15;
                if (m.x > l.a.x - radius && m.x < l.a.x + radius && m.y > l.a.y - radius &&
                    m.y < l.a.y + radius) { // check handle a
                    glfwSetCursor(window, select_cursor);
                    hovered_line_handle = &l.a;
                    hovered_line = nullptr;
                    return;
                } else if (m.x > l.b.x - radius && m.x < l.b.x + radius && m.y > l.b.y - radius &&
                           m.y < l.b.y + radius) { // check handle b
                    glfwSetCursor(window, select_cursor);
                    hovered_line_handle = &l.b;
                    hovered_line = nullptr;
                    return;
                } else { // check if over the line
                    if (l.a.x == l.b.x) {
                        if (m.x > l.a.x - 5 && m.x < l.a.x + 5) {
                            if (l.a.y < l.b.y) {
                                if (m.y > l.a.y && m.y < l.b.y) {
                                    glfwSetCursor(window, select_cursor);
                                    hovered_line_handle = nullptr;
                                    hovered_line = &l;
                                    return;
                                }
                            } else {
                                if (m.y < l.a.y && m.y > l.b.y) {
                                    glfwSetCursor(window, select_cursor);
                                    hovered_line_handle = nullptr;
                                    hovered_line = &l;
                                    return;
                                }
                            }
                        }
                    } else {
                        const vec2 delta = {l.b.x - l.a.x, l.b.y - l.a.y};
                        const float slope = delta.y / delta.x;
                        const vec2 rel_m = m - l.a;
                        if (delta.x < 0.f) {
                            if (delta.y < 0.f) {
                                if (m.x > l.b.x && m.x < l.a.x && m.y > l.b.y && m.y < l.a.y) { // --
                                    if (m.y - 5 - slope * 4 < rel_m.x * slope + l.a.y)
                                        if (m.y + 5 + slope * 4 > rel_m.x * slope + l.a.y) {
                                            glfwSetCursor(window, select_cursor);
                                            hovered_line_handle = nullptr;
                                            hovered_line = &l;
                                            return;
                                        }
                                }
                            } else {
                                if (m.x > l.b.x && m.x < l.a.x && m.y > l.a.y && m.y < l.b.y) { // -+
                                    if (m.y + 5 - slope * 4 > rel_m.x * slope + l.a.y)
                                        if (m.y - 5 + slope * 4 < rel_m.x * slope + l.a.y) {
                                            glfwSetCursor(window, select_cursor);
                                            hovered_line_handle = nullptr;
                                            hovered_line = &l;
                                            return;
                                        }
                                }
                            }
                        } else {
                            if (delta.y < 0.f) {
                                if (m.x > l.a.x && m.x < l.b.x && m.y > l.b.y && m.y < l.a.y) { // +-
                                    if (m.y + 5 - slope * 4 > rel_m.x * slope + l.a.y)
                                        if (m.y - 5 + slope * 4 < rel_m.x * slope + l.a.y) {
                                            glfwSetCursor(window, select_cursor);
                                            hovered_line_handle = nullptr;
                                            hovered_line = &l;
                                            return;
                                        }
                                }
                            } else {
                                if (m.x > l.a.x && m.x < l.b.x && m.y > l.a.y && m.y < l.b.y) { // ++
                                    if (m.y - 5 - slope * 4 < rel_m.x * slope + l.a.y)
                                        if (m.y + 5 + slope * 4 > rel_m.x * slope + l.a.y) {
                                            glfwSetCursor(window, select_cursor);
                                            hovered_line_handle = nullptr;
                                            hovered_line = &l;
                                            return;
                                        }
                                }
                            }
                        }
                    }
                }
            }
            glfwSetCursor(window, nullptr);
            hovered_line_handle = nullptr;
            hovered_line = nullptr;
        }
}
static inline void set_line_point1() {
    place_mode = true;
    lines.push_back({get_grid_mouse() - cam_pos, get_grid_mouse() - cam_pos});
}
static inline void set_line_point2() {
    tool_id = 1;
    place_mode = false;
    glfwSetCursor(window, nullptr);
    lines.back().b = get_grid_mouse() - cam_pos;
}
static inline void drag_line_point2() {
    if (lines.size()) { lines.back().b = get_grid_mouse() - cam_pos; }
}
static inline void grab_line_handle() {
    if (hovered_line_handle) {
        grabbed_line_handle = true;
        *hovered_line_handle = get_grid_mouse() - cam_pos;
    }
}
static inline void place_line_handle() {
    grabbed_line_handle = false;
    if (hovered_line_handle) *hovered_line_handle = get_grid_mouse() - cam_pos;
}
static inline void drag_line_handle() {
    if (grabbed_line_handle && hovered_line_handle) *hovered_line_handle = get_grid_mouse() - cam_pos;
}
static inline void drag_line() {
    if (grabbed_line && hovered_line) {
        hovered_line->a = mouse - grabbed_line_offset_a - cam_pos;
        hovered_line->b = mouse - grabbed_line_offset_b - cam_pos;
        if (snap_to_grid) {
            hovered_line->a = {10.f * int(hovered_line->a.x / 10), 10.f * int(hovered_line->a.y / 10)};
            hovered_line->b = {10.f * int(hovered_line->b.x / 10), 10.f * int(hovered_line->b.y / 10)};
        }
    }
}
static inline void grab_line() {
    if (hovered_line) {
        grabbed_line = true;
        grabbed_line_offset_a = mouse - cam_pos - hovered_line->a;
        grabbed_line_offset_b = mouse - cam_pos - hovered_line->b;
        drag_line();
    }
}
static inline void place_line() {
    drag_line();
    grabbed_line = false;
}

static inline void hover_rects() {
    if (rects.size())
        if (!grabbed_rect_handle) {
            for (Rect &r : rects) {
                const vec2 m = mouse - cam_pos;
                constexpr float radius = 10;
                if (m.x > r.pos.x - radius && m.x < r.pos.x + radius && m.y > r.pos.y - radius && m.y < r.pos.y + radius) {
                    hovered_rect = &r;
                    rect_handle_id = 0;
                    return;
                } else if (m.x > r.pos.x + r.size.x - radius && m.x < r.pos.x + r.size.x + radius &&
                           m.y > r.pos.y + r.size.y - radius && m.y < r.pos.y + r.size.y + radius) {
                    hovered_rect = &r;
                    rect_handle_id = 1;
                    return;
                }
            }
            hovered_rect = nullptr;
        }
}
static inline void set_rect_point1() {
    //
}
static inline void set_rect_point2() {
    //
}
static inline void drag_rect_point2() {
    //
}
static inline void grab_rect_handle() {
    //
}
static inline void place_rect_handle() {
    //
}
static inline void drag_rect_handle() {
    //
}

static inline void drag_camera() {
    if (cam_move) { set_cam_pos(mouse - temp_cam_pos); }
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

static void window_resize(GLFWwindow *window, int w, int h) {
    window_width = w, window_height = h;
    glViewport(0, 0, w, h);
    glUniform2f(glGetUniformLocation(s_shader, "window_size"), w, h);
}
static void mouse_move(GLFWwindow *window, double x, double y) {
    mouse.x = x, mouse.y = window_height - y;
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
static void mouse_press(GLFWwindow *window, int button, int action, int mods) {
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
static void key_press(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_D:
        switch (action) {
        case GLFW_PRESS:
            glfwSetCursor(window, select_cursor);
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

static inline void draw_viewport() {
    // Grid
    draw_rect({menu_width, 0}, {window_width - menu_width, window_height}, 6);

    // Items (only segments at the moment)
    constexpr float handle_radius = 4, handle_width = 2 * handle_radius;
    const float x_cam_off = cam_pos.x, y_cam_off = cam_pos.y;
    for (const Line &l : lines) // The lines
        draw_line({l.a.x + x_cam_off, l.a.y + y_cam_off}, {l.b.x + x_cam_off, l.b.y + y_cam_off}, handle_radius, line_color);
    if (hovered_line) { // selected
        const Line &l = *hovered_line;
        draw_line({l.a.x + x_cam_off, l.a.y + y_cam_off}, {l.b.x + x_cam_off, l.b.y + y_cam_off}, 7, selection_color2);
    }
    for (const Line &l : lines) { // points
        draw_circle({l.a.x + x_cam_off, l.a.y + y_cam_off}, handle_radius, handle_color);
        draw_circle({l.b.x + x_cam_off, l.b.y + y_cam_off}, handle_radius, handle_color);
    }
    if (hovered_line_handle) { // selected
        const vec2 &p = *hovered_line_handle;
        draw_circle({p.x + x_cam_off, p.y + y_cam_off}, 8, selection_color2);
    }
    if (tool_id == 2) draw_circle(get_grid_mouse(), 5, selection_color1);
}

static inline void draw_menu_button(const std::string &string, const float height, const color &bcol, const color &tcol) {
    const float len = string.length();
    draw_rect(get_button_pos(len, height), get_button_size(len), 5, bcol);
    draw_text(menu_width / 2 - len * 4, window_height - height, string, tcol);
}

static inline void draw_menu() {
    draw_rect({menu_width, 0}, {2, window_height}, 5.f, menu_border_color);
    draw_rect({0, 0}, {menu_width, window_height}, 5.f, menu_color);
    draw_rect({0, window_height - 32}, {menu_width, 2}, 5.f, menu_border_color);

    draw_text(menu_width / 2 - 2 * 8, window_height - 24, "Menu", text_color1);

    draw_menu_button("Line Tool", 80, button_color1, text_color1);
    draw_menu_button("Other Tool", 120, button_color1, text_color1);
    draw_menu_button("Other Tool", 160, button_color1, text_color1);
    draw_menu_button("Other Tool", 200, button_color1, text_color1);

    std::string camera_pos_str = "Camera: " + std::to_string(cam_pos.x) + ", " + std::to_string(cam_pos.y);
    draw_line({menu_width + 24, 17}, {menu_width + 16 + camera_pos_str.length() * 8, 17}, 10, drop_shadow_color1);
    draw_text(menu_width + 20, 10, camera_pos_str, text_color1);
}

static inline void draw_scene() {
    draw_viewport();
    draw_menu();
}

int main() {
    glfwInit();
    window = glfwCreateWindow(window_width, window_height, "Physics", nullptr, nullptr);
    drag_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    select_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    if (!window) {
        debug::error("Failed to create window");
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetCursorPosCallback(window, mouse_move);
    glfwSetMouseButtonCallback(window, mouse_press);
    glfwSetWindowSizeCallback(window, window_resize);
    glfwSetKeyCallback(window, key_press);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    begin();

    while (!glfwWindowShouldClose(window)) {

        draw_scene();
        flush();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}
