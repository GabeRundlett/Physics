#pragma once
#include "Renderer/Renderer.hpp"

#include "UI/Line.hpp"
#include "UI/Rect.hpp"
#include "UI/Button.hpp"

#define DARK_THEME true

#if DARK_THEME
static color
    menu_color = {25, 25, 28, 255},
    menu_border_color = {20, 20, 22, 255},
    text_color1 = {168, 168, 172, 255}, drop_shadow_color1 = {0, 0, 0, 175},
    button_color1 = {22, 22, 24, 255},
    selection_color1 = {100, 100, 200, 255}, selection_color2 = {200, 200, 255, 25},
    line_color = {61, 61, 66, 255}, handle_color = {82, 82, 91, 255};
#else
static color
    menu_color = {255, 255, 255, 255},
    menu_border_color = {200, 200, 200, 255},
    text_color1 = {50, 50, 50, 255}, drop_shadow_color1 = {255, 255, 255, 75},
    button_color1 = {210, 210, 210, 255},
    selection_color1 = {100, 100, 200, 255}, selection_color2 = {200, 200, 255, 25},
    line_color = {180, 180, 180, 255}, handle_color = {150, 150, 150, 255};
#endif

static inline void draw_viewport() {
    // Grid
    draw_rect({
        .pos = {menu_width, 0},
        .size = {
            static_cast<float>(window_width) - menu_width,
            static_cast<float>(window_height),
        },
        .mid = 6,
    });

    // Items (only segments at the moment)
    constexpr float handle_radius = 4;
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
    if (tool_id == 2)
        draw_circle(get_grid_mouse(), 5, selection_color1);
}
static inline void draw_menu_button(const std::string &string, const float height, const color &bcol, const color &tcol) {
    const float len = static_cast<float>(string.length());
    draw_rect({
        .pos = get_button_pos(len, height),
        .size = get_button_size(len),
        .col = bcol,
        .mid = 5,
    });
    draw_text(menu_width / 2 - len * 4, static_cast<float>(window_height) - height, string, tcol);
}
static inline void draw_menu() {
    draw_rect({
        .pos = {menu_width, 0},
        .size = {2, static_cast<float>(window_height)},
        .col = menu_border_color,
        .mid = 5.f,
    });
    draw_rect({
        .pos = {0, 0},
        .size = {menu_width, static_cast<float>(window_height)},
        .col = menu_color,
        .mid = 5.f,
    });
    draw_rect({
        .pos = {0, static_cast<float>(window_height) - 32},
        .size = {menu_width, 2},
        .col = menu_border_color,
        .mid = 5.f,
    });

    draw_text(menu_width / 2 - 2 * 8, static_cast<float>(window_height) - 24, "Menu", text_color1);

    draw_menu_button("Line Tool [d]", 80, button_color1, text_color1);

    std::string text = "Camera: " + std::to_string(cam_pos.x) + ", " + std::to_string(cam_pos.y);
    draw_line({menu_width + 24, 17}, {menu_width + 16 + static_cast<float>(text.length()) * 8, 17}, 10, drop_shadow_color1);
    draw_text(menu_width + 20, 10, text, text_color1);

    text = "Frame time (avg): " + std::to_string(avg_time) + ", (current): " + std::to_string(elapsed_time);
    draw_line({menu_width + 24, 37}, {menu_width + 16 + static_cast<float>(text.length()) * 8, 37}, 10, drop_shadow_color1);
    draw_text(menu_width + 20, 30, text, text_color1);
}
static inline void draw_scene() {
    draw_viewport();
    draw_menu();
}

#include "Tools.inl"
