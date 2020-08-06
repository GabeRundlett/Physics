#pragma once
static constexpr const char *const sFragSrc = R"(
#version 430

in vec2 v_size;
in vec2 v_tex;
in vec4 v_col;
in float v_mid;
in float v_data1;

uniform vec2 window_size = vec2(1600, 900);
uniform vec2 cam_pos = vec2(0, 0);
uniform vec2 viewport_center = vec2(900, 450);
out vec4 color;

float line(in vec2 pos, in vec2 a, in vec2 b, in float radius) {
	vec2 ab = a - b;
	float t = clamp(dot(pos - b, ab) / dot(ab, ab), 0, 1);
	return radius - length(pos - b - t * ab);
}

uniform sampler2D tex;

void main() {
	if (v_mid > 0.5 && v_mid < 1.5) { 
		color = vec4(0, 0, 0, 1);
	} else if (v_mid < 2.5) { // -------------------------------------------------- CIRCLE (mid: 2)
		float alpha = clamp((0.51 - length(v_tex - 0.5)) * v_size.x  * 0.8, 0, 1);
		color = vec4(v_col.xyz, alpha * v_col.w);
	} else if (v_mid < 3.5) { // -------------------------------------------------- LINE (mid: 3)
		float radius = v_data1;
		vec2 size = abs(v_size);
		vec2 uv = v_tex * size;
		float alpha = clamp(line(uv, vec2(radius), size - vec2(radius), radius), 0, 1);
		color = vec4(v_col.xyz, alpha * v_col.w);
	} else if (v_mid < 4.5) { // -------------------------------------------------- TEXT (mid: 4)
		color = texture(tex, v_tex) * v_col;
	} else if (v_mid < 5.5) { // -------------------------------------------------- FILL COLOR (mid: 5)
		color = v_col;
	} else if (v_mid < 6.5) { // -------------------------------------------------- BACKGROUND GRID (mid: 6)
		vec2 uv = gl_FragCoord.xy - cam_pos - viewport_center;
		vec4 col1 = vec4(.04, .04, .044, 1), col2 = vec4(.1, .1, .11, 1), col3 = vec4(0.12, 0.12, 0.132, 1);
		// vec4 col1 = vec4(.7, .7, .7, 1), col2 = vec4(.8, .8, .8, 1), col3 = vec4(.9, .9, .9, 1);
		if (int(uv.x) % 100 == 0 && uv.x > 0) color = col1;
		else if (int(uv.x-100 + 3) % 100 == 0 && uv.x < 0) color = col1;
		else if (int(uv.y) % 100 == 0 && uv.y > 0) color = col1;
		else if (int(uv.y-100 + 3) % 100 == 0 && uv.y < 0) color = col1;
		else if (int(uv.x) % 10 == 0 && uv.x > 0) color = col2;
		else if (int(uv.x-10 + 3) % 10 == 0 && uv.x < 0) color = col2;
		else if (int(uv.y) % 10 == 0 && uv.y > 0) color = col2;
		else if (int(uv.y-10 + 3) % 10 == 0 && uv.y < 0) color = col2;
		else color = col3;
	} else { // DEFAULT
		color = vec4(1, 0, 1, 1);
	}
	// color.w = clamp(color.w, 0.5, 1.0);
}
)";
