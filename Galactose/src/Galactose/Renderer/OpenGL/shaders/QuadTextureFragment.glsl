R"(
#version 410 core

layout(location = 0) in vec2 i_uv;

layout(location = 0) out vec4 o_color;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main() {
	o_color = u_color * texture(u_texture, i_uv);
}
)"