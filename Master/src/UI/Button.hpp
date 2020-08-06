#pragma once

static inline vec2 get_button_pos(const float length, const float height) {
    return {menu_width / 2 - length * 4 - button_margin.x, window_height - height - button_margin.y};
}
static constexpr inline vec2 get_button_size(const float length) {
    return {length * 8 + button_margin.x * 2, 16 + button_margin.y * 2};
}
