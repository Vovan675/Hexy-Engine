#version 440

in vec2 uv;
out vec4 color;

uniform sampler2DMS u_texture;

vec4 MultisampleTexture(sampler2DMS tex, ivec2 texCoord, int samples)
{
	vec4 c = vec4(0.0);
	for (int i = 0; i < samples; i++)
		c += texelFetch(tex, texCoord, i);

	return c / float(samples);
}
void main() {
	//vec2 relPos = gl_FragCoord.xy / u_resolution - 0.5;
	//float vignette = smoothstep(0.65, 0.4, length(relPos));
	//color = mix(texture(u_texture, uv), texture(u_texture, uv) * vignette, 0.6);
	//vec3 c = texture(u_texture, uv).rgb;

	ivec2 texCoord = ivec2(uv * ivec2(textureSize(u_texture)));
	vec3 c = MultisampleTexture(u_texture, texCoord, 8).rgb;

	c = pow(c, vec3(1.0/2.2));  

	color = vec4(c, 1);
}