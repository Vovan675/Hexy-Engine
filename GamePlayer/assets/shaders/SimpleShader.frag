#version 430

in vec3 pos;
in vec2 uv;
in vec3 normal;
in mat3 tangentSpace;

out vec4 color;

uniform vec3 u_color;

uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_MetalnessTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_AOTexture;

uniform vec3 u_Albedo;
uniform float u_Metalness;
uniform float u_Roughness;

uniform float u_useAlbedoTexture;
uniform float u_useMetalnessTexture;
uniform float u_useRoughnessTexture;
uniform float u_useNormalTexture;
uniform float u_useAOTexture;

uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;
uniform sampler2D u_brdfMap;

uniform vec3 u_cameraPosition;

//#define LightPos vec3(2, 1, 1)
#define LightDir vec3(0, 0.5, 0.5)
#define LightColor vec3(1)
#define PI 3.14159265359

vec3 N;
vec3 V;
float NdotV;

vec3 albedo;
float metallic;
float roughness;
float ao;

//N - normal; H - halfWay vector;
float Trowbridge(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = clamp(NdotH * NdotH, 0.0, 1.0);

	float delim = NdotH2 * (a2 - 1.0) + 1.0;
	float delim2 = delim * delim;

	float D = a2 / max(PI * delim2, 0.0000001);
	return D;
}

//cosTheta - viewDir vs halfWay angle; F0 - metallness;
vec3 FreshelSchlick(float cosTheta, vec3 F0) {
	//float F0 = pow((n1 - n2) / (n1 + n2), 2);
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//cosTheta - viewDir vs halfWay angle; F0 - metallness;
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


//N - normal; V - viewPos (But also lightPos in smith function);
float GeometrySchlick(vec3 N, vec3 V, float roughness) {
	float a = roughness + 1.0;
	float K = (a * a) / 8.0;
	float NdotV = max(dot(N, V), 0.0);
	float G = NdotV / ((NdotV * (1.0-K) + K));
	return G;
}


//N - normal; V - viewPos; L - lightPos;
float Smith(vec3 N, vec3 V, vec3 L, float roughness) {
	float G = GeometrySchlick(N, L, roughness) * GeometrySchlick(N, V, roughness);
	return G;
}


vec3 Lighting(vec3 F0) {
	vec3 L = normalize(LightDir);//Directional light

	//Cook-Torrance
	vec3 H = normalize(V + L);
	float D = Trowbridge(N, H, roughness);

	float HdotV = clamp(dot(H, V), 0.0, 1.0);
	vec3 F = FreshelSchlick(HdotV, F0);

	float G = Smith(N, V, L, roughness);


	float NdotL = max(dot(N, L), 0.0);

	vec3 fCook = (D * F * G) / max((4.0 * NdotL * NdotV), 0.0000001); //Max for handling division by 0

	vec3 kS = F; //kS in BRDF function (specular coeff (Specular))
	vec3 kD = vec3(1.0) - kS; //kD in BRDF function (refracted coeff (Diffuce))
	kD *= 1.0 - metallic; //Because fully metallness surfaces dont refract any light

	//float dist = distance(LightPos, pos);//Point light
	//float attenuation = 1.0 / (dist * dist);//Point light
	vec3 radiance = LightColor * NdotL;
	vec3 Lo = (kD * albedo / PI + fCook) * radiance;
	return vec3(Lo);
}

vec3 IBL(vec3 F0) {
	vec3 kS = FresnelSchlickRoughness(NdotV, F0, roughness);
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
	vec3 irradiance = texture(u_irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;


	vec3 R = reflect(-V, N);
	const float MAX_REFLECTION_LOD = textureQueryLevels(u_prefilterMap)-1.0;
	vec3 prefilteredColor = textureLod(u_prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(u_brdfMap, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);

	vec3 ibl = (kD * diffuse + specular) * ao;
	return ibl;
}

void main() {

	albedo = u_useAlbedoTexture > 0.5 ? texture2D(u_AlbedoTexture, uv).rgb : u_Albedo;
	metallic = u_useMetalnessTexture > 0.5 ? texture2D(u_MetalnessTexture, uv).r : u_Metalness;
	roughness = u_useRoughnessTexture > 0.5 ? texture2D(u_RoughnessTexture, uv).r : u_Roughness;
	roughness = max(roughness, 0.05);
	ao = u_useAOTexture > 0.5 ? texture2D(u_AOTexture, uv).r : 1;
	
	if(u_useNormalTexture > 0.5) {
		N = texture2D(u_NormalTexture, uv).rgb;//[0, 1], but normals should be [-1, 1]
		N = normalize(N * 2 - 1);//[-1, 1]
		N = normalize(tangentSpace * N);
	}
	else {
		N = normalize(normal);
	}
	
	V = normalize(u_cameraPosition - pos);
	NdotV = clamp(dot(N, V), 0.0, 1.0);


	vec3 F0 = vec3(0.04); //Basic aproximate for dielectric
	F0 = mix(F0, albedo, metallic);

	
	vec3 light = Lighting(F0);
	vec3 ibl = IBL(F0);

	
	//////////////

	vec3 final = light + ibl;
	//final = irradiance;

	//HDR + TONE MAPPING
	//final = final / (final + vec3(1.0));
	//final = vec3(1.0) - exp(-final * 1);
	//final = pow(final, vec3(1.0/2.2));  
	
	color = vec4(final, 1);
	
	
	//color = vec4(irradiance, 1);

	////color = vec4(u_color, 1);
	//color = texture(u_brdfMap, vec2(NdotV, roughness));
	//color = texture(u_prefilterMap, reflect(-V, N));
	//color = texture2D(u_AOTexture, uv);
	//color = vec4(1, 1, 1, 1);
	//color = vec4(uv, 0, 1.0);
	//color = vec4(normal, 1.0);
	//color = vec4(1, 0, 1, 1.0);
}