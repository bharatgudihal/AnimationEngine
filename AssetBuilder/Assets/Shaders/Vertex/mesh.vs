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

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec3 ObjectColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ObjectColor = aColor;
	TexCoord = aTexCoord;
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	Normal = normalMatrix * aNormal;	
	vec3 T = normalize(normalMatrix * aTangent);	
	vec3 N = normalize(Normal);

	//re-orthogonalize T wrt N
	T = normalize(T - dot(T,N) * N);
	
	//Recalculate B
	vec3 B = cross(N, T);
	TBN = mat3(T,B,N);
	FragPos = vec3(model * vec4(aPos, 1.0));
}