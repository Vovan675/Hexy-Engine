#version 330
layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_uv;

out vec2 uv;

uniform mat4 u_cameraMatrix;
uniform mat4 u_modelMatrix;

void main() {
	gl_Position = u_cameraMatrix * u_modelMatrix * vec4(a_pos, 1.0);
	//gl_Position = u_modelMatrix * vec4(a_pos, 0, 1.0);
	uv = a_uv;
}