#pragma once
#include "Interface.hpp"
#include <vector>

struct Rect {
    vec2 pos, size;
};
static std::vector<Rect> rects;

static bool grabbed_rect_handle = false;
static unsigned char rect_handle_id = 0;
static Rect *hovered_rect = nullptr;

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
