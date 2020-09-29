#version 430
layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_uv;
layout(location=2) in vec3 a_normal;
layout(location=3) in vec3 a_tangent;

out vec3 pos;
out vec2 uv;
out vec3 normal;
out mat3 tangentSpace;

uniform mat4 u_cameraMatrix;
uniform mat4 u_modelMatrix;

void main() {
	gl_Position = u_cameraMatrix * u_modelMatrix * vec4(a_pos, 1.0);
	//gl_Position = u_modelMatrix * vec4(a_pos, 0, 1.0);
	pos = vec3(u_modelMatrix * vec4(a_pos, 1.0)); //World position
	uv = a_uv;
	normal = normalize(mat3(u_modelMatrix) * a_normal); //World position

	vec3 bitangent = -normalize(cross(a_normal, a_tangent));
	tangentSpace = mat3(u_modelMatrix) * mat3(a_tangent, bitangent, a_normal);
}