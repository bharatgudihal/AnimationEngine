#version 420 core

struct Light {
	vec4 vector;
	vec4 direction;
	
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
		
	float linear;
	float quadratic;

	float isSpotLight;
	float innerCutOff;
	float outerCutOff;

	vec3 padding;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

layout (std140, binding = 0) uniform dataPerFrame {
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	
	Light light;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 ObjectColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ObjectColor = aColor;
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}