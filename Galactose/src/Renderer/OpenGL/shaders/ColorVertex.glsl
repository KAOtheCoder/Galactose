R"(
#version 410 core

layout(location = 0) in vec3 i_position;

uniform mat4 u_viewProjection;

void main() {
	gl_Position = u_viewProjection * vec4(i_position, 1.0);
}
)"