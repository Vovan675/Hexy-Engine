#version 330

in vec3 pos;

out vec4 color;


uniform samplerCube u_cubeMap;


void main() {
	vec3 c = textureLod(u_cubeMap, normalize(pos), 0).rgb;
	//c = c / (c + vec3(1.0));
    //c = pow(c, vec3(1.0/2.2)); 
	color = vec4(c, 1);
}