#include "OpenGL.hpp"

#include "Shaders/Frag.hpp"
#include "Shaders/Vert.hpp"

struct vec2 {
    float x, y;
    vec2 operator+(const vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }
    vec2 operator-(const vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }
    vec2 operator*(const float rhs) const { return {x * rhs, y * rhs}; }
    vec2 operator/(const float rhs) const { return {x / rhs, y / rhs}; }
};
struct color {
    unsigned char r, g, b, a;
};

struct Vertex {
    vec2 pos, size, tex;
    color col;
    float mid, data1;
};

static constexpr unsigned int s_sprite_count = 10000, s_vbuffer_size = 4 * sizeof(Vertex) * s_sprite_count,
                              s_ibuffer_size = 6 * s_sprite_count;
static unsigned int s_vao_id, s_vbo_id, s_ibo_id;
static unsigned short s_indices[s_ibuffer_size];
static unsigned int s_index_count = 0;
static Vertex *s_vbuffer_pointer;
static int s_shader = 0, s_texture = 0;

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)8);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)16);
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (const void *)24);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)28);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)32);

    for (unsigned int i = 0; i < s_sprite_count; ++i) {
        const unsigned int index = i * 6, offset = i * 4;
        s_indices[index] = offset;
        s_indices[index + 1] = 1 + offset;
        s_indices[index + 2] = 2 + offset;
        s_indices[index + 3] = 1 + offset;
        s_indices[index + 4] = 3 + offset;
        s_indices[index + 5] = 2 + offset;
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

static inline void draw_rect(const vec2 &pos, const vec2 &size, float mid, const color &col = {0, 0, 0, 255},
                             const vec2 &tex_pos = {0.f, 0.f}, const vec2 &tex_size = {1.f, 1.f}) {
    s_vbuffer_pointer->pos.x = pos.x;
    s_vbuffer_pointer->pos.y = pos.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = pos.x;
    s_vbuffer_pointer->pos.y = pos.y + size.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = pos.x + size.x;
    s_vbuffer_pointer->pos.y = pos.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = pos.x + size.x;
    s_vbuffer_pointer->pos.y = pos.y + size.y;
    s_vbuffer_pointer->size = size;
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_index_count += 6;
}

static inline void draw_rect_ab(const vec2 &a, const vec2 &b, float mid, const color &col = {0, 0, 0, 255},
                                const vec2 &tex_pos = {0.f, 0.f}, const vec2 &tex_size = {1.f, 1.f}) {
    s_vbuffer_pointer->pos.x = a.x;
    s_vbuffer_pointer->pos.y = a.y;
    s_vbuffer_pointer->size = a - b;
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = a.x;
    s_vbuffer_pointer->pos.y = b.y;
    s_vbuffer_pointer->size = a - b;
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = b.x;
    s_vbuffer_pointer->pos.y = a.y;
    s_vbuffer_pointer->size = a - b;
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = b.x;
    s_vbuffer_pointer->pos.y = b.y;
    s_vbuffer_pointer->size = a - b;
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = mid;
    s_vbuffer_pointer->data1 = 0.f;
    ++s_vbuffer_pointer;
    s_index_count += 6;
}

static inline void draw_line(const vec2 &a, const vec2 &b, const float radius, const color &col = {0, 0, 0, 255},
                             const vec2 &tex_pos = {0.f, 0.f}, const vec2 &tex_size = {1.f, 1.f}) {
    float x_border = radius;
    if (b.x < a.x)
        x_border *= -1;
    float y_border = radius;
    if (b.y < a.y)
        y_border *= -1;
    s_vbuffer_pointer->pos.x = a.x - x_border;
    s_vbuffer_pointer->pos.y = a.y - y_border;
    s_vbuffer_pointer->size = a - b - vec2{x_border * 2, y_border * 2};
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = 3.f;
    s_vbuffer_pointer->data1 = radius;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = a.x - x_border;
    s_vbuffer_pointer->pos.y = b.y + y_border;
    s_vbuffer_pointer->size = a - b - vec2{x_border * 2, y_border * 2};
    s_vbuffer_pointer->tex.x = tex_pos.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = 3.f;
    s_vbuffer_pointer->data1 = radius;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = b.x + x_border;
    s_vbuffer_pointer->pos.y = a.y - y_border;
    s_vbuffer_pointer->size = a - b - vec2{x_border * 2, y_border * 2};
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = 3.f;
    s_vbuffer_pointer->data1 = radius;
    ++s_vbuffer_pointer;
    s_vbuffer_pointer->pos.x = b.x + x_border;
    s_vbuffer_pointer->pos.y = b.y + y_border;
    s_vbuffer_pointer->size = a - b - vec2{x_border * 2, y_border * 2};
    s_vbuffer_pointer->tex.x = tex_pos.x + tex_size.x;
    s_vbuffer_pointer->tex.y = tex_pos.y + tex_size.y;
    s_vbuffer_pointer->col = col;
    s_vbuffer_pointer->mid = 3.f;
    s_vbuffer_pointer->data1 = radius;
    ++s_vbuffer_pointer;
    s_index_count += 6;
}

static inline void draw_circle(const vec2 &p, const float radius, const color &col = {0, 0, 0, 255}) {
    draw_rect({p.x - radius, p.y - radius}, {radius * 2, radius * 2}, 2, col);
}

static inline void draw_text(float x, float y, const std::string &text, const color &col) {
    for (unsigned int i = 0; i < text.length(); ++i) {
        const int index = static_cast<int>(text[i]) - 32;
        const float u = static_cast<float>(index) / 223;
        draw_rect({x + 8.f * i, y}, {8.f, 16.f}, 4.0, col, {u, 0.f}, {1.f / 223, 1.f});
    }
}
