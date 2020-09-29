#version 330
layout(location=0) in vec3 a_pos;

out vec3 pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
	pos = a_pos; //local position

	mat4 rotView = mat4(mat3(u_view));
	vec4 p = u_projection * rotView * vec4(a_pos, 1.0);

	gl_Position = p.xyww;
}