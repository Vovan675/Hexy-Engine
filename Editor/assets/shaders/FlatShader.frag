#version 330

in vec2 uv;
out vec4 color;

uniform vec3 u_color;
uniform sampler2D u_texture;
void main() {
	color = texture2D(u_texture, uv) * vec4(u_color, 1);
	//color = texture2D(u_texture, uv);
	//color = vec4(uv, 0, 1.0);
}
