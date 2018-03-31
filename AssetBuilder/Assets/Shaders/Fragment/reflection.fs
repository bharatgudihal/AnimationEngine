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

in vec4 worldPosition;
in vec4 worldNormal;

out vec4 FragColor;

uniform samplerCube skyBox;

void main(){
	vec3 viewDirection = normalize(vec3(worldPosition - viewPos));
	vec3 reflectedVector = reflect(viewDirection, normalize(vec3(worldNormal)));
	FragColor = texture(skyBox, reflectedVector);
}