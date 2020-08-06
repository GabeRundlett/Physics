#pragma once
#include <vector>

struct Line {
    vec2 a, b;
};
static std::vector<Line> lines;

static bool grabbed_line = false, grabbed_line_handle = false;
static vec2 *hovered_line_handle = nullptr, grabbed_line_offset_a, grabbed_line_offset_b;
static Line *hovered_line = nullptr;

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
    if (lines.size()) {
        lines.back().b = get_grid_mouse() - cam_pos;
    }
}
static inline void grab_line_handle() {
    if (hovered_line_handle) {
        grabbed_line_handle = true;
        *hovered_line_handle = get_grid_mouse() - cam_pos;
    }
}
static inline void place_line_handle() {
    grabbed_line_handle = false;
    if (hovered_line_handle)
        *hovered_line_handle = get_grid_mouse() - cam_pos;
}
static inline void drag_line_handle() {
    if (grabbed_line_handle && hovered_line_handle)
        *hovered_line_handle = get_grid_mouse() - cam_pos;
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
