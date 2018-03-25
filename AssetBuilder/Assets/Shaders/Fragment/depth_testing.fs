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
	sampler2D diffuse;
	sampler2D specular;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
	bool hasDiffuse;
	bool hasSpecular;
};

#define NR_POINT_LIGHTS 4
layout (std140, binding = 0) uniform dataPerFrame {
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	
	DirectionalLight directionalLight;

	PointLight pointLights[NR_POINT_LIGHTS];

	SpotLight spotLight;
};

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

float nearPlane = 0.1;
float farPlane = 100.0;

float LinearizeDepth(float fragDepth){
	float z = fragDepth * 2.0 - 1.0;
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / farPlane;
	FragColor = vec4(vec3(depth),1.0);
}