#version 330
//Use vertex shader the same as for texture_to_cube.frag
in vec3 pos;

out vec4 color;

uniform samplerCube u_cubeMap;

#define PI 3.14159265359

void main() {
	//Because cube position is (0, 0, 0), this position can be used as normal to sphere surface
	vec3 N = normalize(pos);

	vec3 irradiance = vec3(0);
	vec3 up = vec3(0, 1, 0);
	vec3 right = normalize(cross(up, N));
	up = cross(N, right);

	float sampleDelta = 0.025;
    float nrSamples = 0.0; 
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            //spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            //tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(u_cubeMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
	color = vec4(irradiance, 1.0);

}