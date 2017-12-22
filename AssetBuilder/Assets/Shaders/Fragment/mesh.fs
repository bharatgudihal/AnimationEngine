#version 420 core

struct Light {
	vec4 vector;
	
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
		
	float linear;
	float quadratic;
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

	vec2 padding;
};

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

void main()
{
	vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));
	vec3 matSpecular = vec3(texture(material.specular, TexCoord));

	vec3 lightDirection;
	float attenuation = 1.0;

	// check if it's a directional light
	if(light.vector.w == 0.0){
		lightDirection = normalize(-vec3(light.vector));
	}else{
		lightDirection = normalize(vec3(light.vector) - FragPos);
		float distance = length(vec3(light.vector) - FragPos);
		attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));
	}

	//ambient
	vec3 ambient = vec3(light.ambient) * matDiffuse;

	//diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = vec3(light.diffuse) * diff * matDiffuse;

	//specular
	vec3 viewDirection = normalize(vec3(viewPos) - FragPos);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
	vec3 specular = vec3(light.specular) * spec * matSpecular;

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}