#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 ObjectColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;

layout (std140, binding = 0) uniform dataPerFrame{
	mat4 view;
	mat4 projection;
	vec3 viewPos;
	float a;
	vec3 lightPosition;
	float b;
	vec3 lightAmbient;
	float c;
	vec3 lightDiffuse;
	float d;
	vec3 lightSpecular;
	float e;
};

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ObjectColor = aColor;
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}