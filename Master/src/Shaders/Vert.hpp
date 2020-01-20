#pragma once
static constexpr const char *const sVertSrc = R"(
#version 430

layout(location=0) in vec2 pos;
layout(location=1) in vec2 size;
layout(location=2) in vec2 tex;
layout(location=3) in vec4 col;
layout(location=4) in float mid;
layout(location=5) in float data1;

uniform vec2 window_size = vec2(1600, 900);

out vec2 v_size;
out vec2 v_tex;
out vec4 v_col;
out float v_mid;
out float v_data1;

void main() {
	v_size = size;
	v_tex = tex;
	v_col = col / 255;
	v_mid = mid;
	v_data1 = data1;
	gl_Position = vec4(pos / window_size * 2 - 1, 0, 1);
}
)";
