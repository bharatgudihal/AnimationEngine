#version 420 core

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

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

struct Material{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{
	vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));

	//ambient
	vec3 ambient = lightAmbient * matDiffuse;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - FragPos);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = lightDiffuse * diff * matDiffuse;

	//specular
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
	vec3 specular = lightSpecular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}