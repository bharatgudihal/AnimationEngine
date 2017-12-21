#version 420 core

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

layout (std140, binding = 0) uniform dataPerFrame{
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	vec4 lightVector;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
};

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

void main()
{
	vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));
	vec3 matSpecular = vec3(texture(material.specular, TexCoord));

	vec3 lightDirection;

	if(lightVector.w == 0.0f){
		lightDirection = normalize(-vec3(lightVector));
	}else{
		lightDirection = normalize(vec3(lightVector) - FragPos);
	}

	//ambient
	vec3 ambient = vec3(lightAmbient) * matDiffuse;

	//diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = vec3(lightDiffuse) * diff * matDiffuse;

	//specular
	vec3 viewDirection = normalize(vec3(viewPos) - FragPos);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
	vec3 specular = vec3(lightSpecular) * spec * matSpecular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}