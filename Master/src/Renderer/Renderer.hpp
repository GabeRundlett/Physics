#pragma once
#include "OpenGL.hpp"
#include "Shaders/Vert.hpp"
#include "Shaders/Frag.hpp"

struct vec2 {
    float x, y;
    inline constexpr vec2 operator+(const vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }
    inline constexpr vec2 operator-(const vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }
    inline constexpr vec2 operator*(const float rhs) const { return {x * rhs, y * rhs}; }
    inline constexpr vec2 operator/(const float rhs) const { return {x / rhs, y / rhs}; }
};
struct color {
    unsigned char r, g, b, a;
};
struct Vertex {
    vec2 pos, size, tex;
    color col;
    float mid, data1;
};

static constexpr GLsizei s_sprite_count = 10000, s_vbuffer_size = 4 * sizeof(Vertex) * s_sprite_count,
                         s_ibuffer_size = 6 * s_sprite_count;
static unsigned int s_vao_id, s_vbo_id, s_ibo_id;
static unsigned short s_indices[s_ibuffer_size];
static GLsizei s_index_count = 0;
static Vertex *s_vbuffer_pointer;
static unsigned int s_shader = 0, s_texture = 0;

struct rect_config {
    vec2 pos, size;
    vec2 tex_pos = {0, 0}, tex_size = {1, 1};
    color col = {0, 0, 0, 255};
    float mid = 0;
};

struct rect_ab_config {
    vec2 p1, p2, border = {0, 0};
    vec2 tex_p1 = {0, 0}, tex_p2 = {1, 1};
    color col = {0, 0, 0, 255};
    float mid = 0, data1 = 0;
};

// .0121 - raw param
// .0126 - config struct param
// .0154 - indirection
// .0125 - indirection p2
// .

static inline void begin() {
    glGenVertexArrays(1, &s_vao_id);
    glBindVertexArray(s_vao_id);
    glGenBuffers(1, &s_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, s_vbuffer_size, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(8));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(16));
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(24));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(28));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(32));
    for (unsigned short i = 0; i < s_sprite_count; ++i) {
        const auto index = static_cast<unsigned short>(i * 6),
                   offset = static_cast<unsigned short>(i * 4);
        s_indices[index] = offset;
        s_indices[index + 1] = static_cast<unsigned short>(1 + offset);
        s_indices[index + 2] = static_cast<unsigned short>(2 + offset);
        s_indices[index + 3] = static_cast<unsigned short>(1 + offset);
        s_indices[index + 4] = static_cast<unsigned short>(3 + offset);
        s_indices[index + 5] = static_cast<unsigned short>(2 + offset);
    }
    glGenBuffers(1, &s_ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);
    s_vbuffer_pointer = reinterpret_cast<Vertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    s_shader = create_shader(GL_VERTEX_SHADER, sVertSrc, GL_FRAGMENT_SHADER, sFragSrc);
    s_texture = create_texture("Master/img/text32.png");
}
static inline void flush() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(s_vao_id);
    glDrawElements(GL_TRIANGLES, s_index_count, GL_UNSIGNED_SHORT, nullptr);
    s_vbuffer_pointer = reinterpret_cast<Vertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    s_index_count = 0;
}

static inline void draw_rect_ab(const rect_ab_config &conf) {
    if (s_index_count + 6 >= s_ibuffer_size)
        flush();
    const auto size = conf.p2 - conf.p1 + (conf.border * 2);
    s_vbuffer_pointer->pos.x = conf.p1.x - conf.border.x;
    s_vbuffer_pointer->pos.y = conf.p1.y - conf.border.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = conf.tex_p1.x;
    s_vbuffer_pointer->tex.y = conf.tex_p1.y;
    s_vbuffer_pointer->col = conf.col;
    s_vbuffer_pointer->mid = conf.mid;
    s_vbuffer_pointer->data1 = conf.data1;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = conf.p1.x - conf.border.x;
    s_vbuffer_pointer->pos.y = conf.p2.y + conf.border.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = conf.tex_p1.x;
    s_vbuffer_pointer->tex.y = conf.tex_p2.y;
    s_vbuffer_pointer->col = conf.col;
    s_vbuffer_pointer->mid = conf.mid;
    s_vbuffer_pointer->data1 = conf.data1;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = conf.p2.x + conf.border.x;
    s_vbuffer_pointer->pos.y = conf.p1.y - conf.border.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = conf.tex_p2.x;
    s_vbuffer_pointer->tex.y = conf.tex_p1.y;
    s_vbuffer_pointer->col = conf.col;
    s_vbuffer_pointer->mid = conf.mid;
    s_vbuffer_pointer->data1 = conf.data1;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = conf.p2.x + conf.border.x;
    s_vbuffer_pointer->pos.y = conf.p2.y + conf.border.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = conf.tex_p2.x;
    s_vbuffer_pointer->tex.y = conf.tex_p2.y;
    s_vbuffer_pointer->col = conf.col;
    s_vbuffer_pointer->mid = conf.mid;
    s_vbuffer_pointer->data1 = conf.data1;
    ++s_vbuffer_pointer;
    s_index_count += 6;
}
static void draw_rect(const rect_config &conf) {
    draw_rect_ab({
        .p1 = conf.pos,
        .p2 = conf.pos + conf.size,
        .tex_p1 = conf.tex_pos,
        .tex_p2 = conf.tex_pos + conf.tex_size,
        .col = conf.col,
        .mid = conf.mid,
    });
}
static void draw_line(const vec2 &a, const vec2 &b, const float radius, const color &col = {0, 0, 0, 255},
                      const vec2 &tex_a = {0.f, 0.f}, const vec2 &tex_b = {1.f, 1.f}) {
    float x_border = radius;
    if (b.x < a.x)
        x_border *= -1;
    float y_border = radius;
    if (b.y < a.y)
        y_border *= -1;
    draw_rect_ab({
        .p1 = a,
        .p2 = b,
        .border = {x_border, y_border},
        .tex_p1 = tex_a,
        .tex_p2 = tex_b,
        .col = col,
        .mid = 3.0f,
        .data1 = radius,
    });
}
static inline void draw_circle(const vec2 &p, const float radius, const color &col = {0, 0, 0, 255}) {
    draw_rect_ab({
        .p1 = {p.x - radius, p.y - radius},
        .p2 = {p.x + radius, p.y + radius},
        .col = col,
        .mid = 2,
    });
}
static inline void draw_text(float x, float y, const std::string &text, const color &col) {
    for (unsigned int i = 0; i < text.length(); ++i) {
        const int index = static_cast<int>(text[i]) - 32;
        const float u = static_cast<float>(index) / 223;
        draw_rect({
            .pos = {x + 8.f * static_cast<float>(i), y},
            .size = {8.f, 16.f},
            .tex_pos = {u, 0.f},
            .tex_size = {1.f / 223, 1.f},
            .col = col,
            .mid = 4.0,
        });
    }
}
