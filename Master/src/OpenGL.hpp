#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "StbImage.hpp"
#include <gl/glew.h>
#include <gl/wglew.h>
#include <cstdio>

static inline void compile_sources(int id, int shader_type, const char *const src) {
    unsigned int compiled_object_id = glCreateShader(shader_type);
    glShaderSource(compiled_object_id, 1, &src, NULL);
    glCompileShader(compiled_object_id);

    int temp = 0;
    glGetShaderiv(compiled_object_id, GL_COMPILE_STATUS, &temp);
    if (temp == GL_FALSE) {
        temp = 0;
        glGetShaderiv(compiled_object_id, GL_INFO_LOG_LENGTH, &temp);
        std::vector<char> infoLog(temp);
        glGetShaderInfoLog(compiled_object_id, temp, &temp, &infoLog[0]);
        glDeleteShader(compiled_object_id);
        std::printf("%s\n", infoLog.data());
    }

    glAttachShader(id, compiled_object_id);
    glDeleteShader(compiled_object_id);
}
template <typename... T>
static inline void compile_sources(int id, int shader_type, const char *const src, T... args) {
    compile_sources(id, shader_type, src);
    compile_sources(id, args...);
}
template <typename... T>
static inline int create_shader(int shader_type, const char *const src, T... args) {
    int id = glCreateProgram();
    compile_sources(id, shader_type, src, args...);
    glLinkProgram(id);
    glUseProgram(id);
    return id;
}
static inline int create_texture(const char *const filepath) {
    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath, &w, &h, &c, 0);
    unsigned int tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    switch (c) {
    case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data); break;
    case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); break;
    default: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); break;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return tid;
}
