#version 420 core

struct LightData {	
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct DirectionalLight {
	vec4 direction;
				
	LightData lightData;
				
	vec4 isActive;
};

struct PointLight {
	vec4 position;
				
	LightData lightData;

	float linear;
	float quadratic;

	float isActive;

	float padding;
};

struct SpotLight {
	vec4 direction;
	vec4 position;

	LightData lightData;
				
	float innerCutOff;
	float outerCutOff;
				
	float isActive;

	float padding;
};

struct Material {	
	sampler2D normalMap;
	sampler2D albedoMap;	
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D aoMap;
	samplerCube irradianceMap;
	samplerCube prefilterMap;
	sampler2D BRDFLUT;
	vec3 albedoColor;
	float metalness;
	float roughness;
	float ao;
	bool hasAlbedoMap;
	bool hasNormalMap;
	bool hasMetallicMap;
	bool hasRoughnessMap;
	bool hasAoMap;
	bool hasIrradianceMap;
};

#define NR_POINT_LIGHTS 4
layout (std140, binding = 0) uniform dataPerFrame {
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	
	DirectionalLight directionalLight;

	PointLight pointLights[NR_POINT_LIGHTS];

	SpotLight spotLight;

	float gamma;
	vec3 padding;
};

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform Material material;

const float PI = 22.0 / 7.0;

vec3 Fresnel(float cosTheta, vec3 F0);
vec3 FresnelWithRoughness(float cosTheta, vec3 F0, float roughness);
float NormalDistribution(vec3 N, vec3 H, float roughness);
float GeometryFunction(vec3 N, vec3 V, vec3 L, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);

void main()
{
	vec3 albedo = material.albedoColor;
	vec3 norm = Normal;
	float metallic = material.metalness;
	float roughness = material.roughness;
	float ao = material.ao;
	
	if(material.hasAlbedoMap){
		albedo	= pow(texture(material.albedoMap, TexCoord).rgb, vec3(2.2));
	}

	if(material.hasNormalMap){
		norm = texture(material.normalMap, TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
		norm = normalize(TBN * norm);
	}

	if(material.hasMetallicMap){
		metallic  = texture(material.metallicMap, TexCoord).r;
	}

	if(material.hasRoughnessMap){
		roughness = texture(material.roughnessMap, TexCoord).r;
	}

	if(material.hasAoMap){
		ao = texture(material.aoMap, TexCoord).r;
	}
	
	const vec3 N = normalize(norm);
	const vec3 V = normalize(vec3(viewPos) - FragPos);
	const vec3 R = reflect(-V, N);
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	//Getting irradiance based on the reflectance equation. Reference:https://learnopengl.com/PBR/Lighting
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		if(pointLights[i].isActive == 1.0){
			//Calculate radiance
			const vec3 L = normalize(vec3(pointLights[i].position) - FragPos);
			const vec3 H = normalize(L + V);
			float distance = length(vec3(pointLights[i].position) - FragPos);
			float attenuation = 1.0 / (distance * distance);
			vec3 radiance = vec3(pointLights[i].lightData.diffuse) * attenuation;		

			//Calculate Cook-Torrence BRDF. ReferenceL https://learnopengl.com/PBR/Theory
			float NDF = NormalDistribution(N, H, roughness);
			float G = GeometryFunction(N, V, L, roughness);
			vec3 F = Fresnel(clamp(dot(H, V), 0.0, 1.0), F0);
		
			vec3 numerator = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
			vec3 specular = numerator / max(denominator, 0.001);

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;

			float NdotL = max(dot(N, L), 0.0);
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		}
	}

	vec3 ambientColor = vec3(0.03);
	if(material.hasIrradianceMap){
		//Use IBL for ambient lighting
		vec3 F = FresnelWithRoughness(max(dot(N, V), 0.0), F0, roughness);
		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
		vec3 irradiance = texture(material.irradianceMap, N).rgb;
		vec3 diffuse = irradiance * albedo;

		const float MAX_REFLECTION_LOD = 4.0;
		vec3 prefilterdColor = textureLod(material.prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
		vec2 brdf = texture(material.BRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
		vec3 specular = prefilterdColor * (F * brdf.x + brdf.y);

		ambientColor = (kD * diffuse + specular) * ao;
	}else{
		ambientColor = ambientColor * albedo * ao;
	}

	vec3 outColor = ambientColor + Lo;

	//HDR tone mapping
	outColor = outColor / (outColor + vec3(1.0));
	//Gamma correction
	outColor = pow(outColor, vec3(1.0 / 2.2));

	FragColor = vec4(outColor, 1.0);
}

//Fresnel-Schlick approximation. Reference: https://learnopengl.com/PBR/Theory
//Returns the ratio of light that gets reflected
//F0 - base reflectivity
vec3 Fresnel(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelWithRoughness(float cosTheta, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

//Normal distribution function using Trowbridge-Reitz GGX. Reference: https://learnopengl.com/PBR/Theory
//statistically approximates the ratio of microfacets aligned to the halfway vector H
float NormalDistribution(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float numerator = a2;
	float denominator = NdotH2 * (a2 - 1.0) + 1.0;
	denominator = PI * denominator * denominator;

	return numerator / max(denominator, 0.001);	
}

//Geometry function using Smith's method. Reference: https://learnopengl.com/PBR/Theory
//statistically approximates the ratio of microfacets that overshadow each other
float GeometryFunction(vec3 N, vec3 V, vec3 L, float roughness){
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

//Geometry function that uses Schlick-GGX approximation. Reference: https://learnopengl.com/PBR/Theory
float GeometrySchlickGGX(float NdotV, float roughness){
	float r = roughness + 1.0;
	float k = r * r / 8.0;

	float numerator = NdotV;
	float denominator = NdotV * (1.0 - k) + k;

	return numerator / denominator;
}