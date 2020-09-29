#version 330

in vec3 pos;

out vec4 color;

uniform sampler2D u_texture;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 pos) {
	vec2 uv = vec2(atan(pos.z, pos.x), asin(pos.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}


void main() {
	vec2 uv = SampleSphericalMap(normalize(pos));
	color = texture2D(u_texture, uv);
}