R"(
#version 410 core

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_uv;

layout(location = 0) out vec2 o_uv;

uniform vec2 u_canvasSize;

void main() {
	gl_Position = vec4(2.0 * i_position.x / u_canvasSize.x - 1.0, 1.0 - 2.0 * i_position.y / u_canvasSize.y, 0, 1.0);

	o_uv = i_uv;
}
)"