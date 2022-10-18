R"(
#version 410 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec2 i_uv;

layout(location = 0) out vec2 o_uv;

uniform mat4 u_viewProjection;

void main() {
	gl_Position = u_viewProjection * vec4(i_position, 1.0);
	o_uv = i_uv;
}
)"