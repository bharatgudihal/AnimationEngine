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

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

void main()
{
	vec3 matDiffuse = vec3(texture(material.diffuse, TexCoord));

	vec3 fragToLight;
	float attenuation = 1.0;	
	float intensity = 1.0f;
	vec3 diffuse;
	vec3 specular;

	// check if it's a directional light
	if(light.vector.w == 0.0){
		fragToLight = normalize(-vec3(light.vector));
	}else{
		fragToLight = normalize(vec3(light.vector) - FragPos);
		float distance = length(vec3(light.vector) - FragPos);
		attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));
		if(light.isSpotLight == 1.0){
			float theta = dot(fragToLight, -vec3(light.direction));
			float epsilon = light.innerCutOff - light.outerCutOff;
			intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		}
	}

	//ambient
	vec3 ambient = vec3(light.ambient) * matDiffuse;

	//diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, fragToLight), 0.0);
	diffuse = vec3(light.diffuse) * diff * matDiffuse * intensity;

	//specular
	vec3 matSpecular = vec3(texture(material.specular, TexCoord));
	vec3 viewDirection = normalize(vec3(viewPos) - FragPos);
	vec3 reflectedDirection = reflect(-fragToLight, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
	specular = vec3(light.specular) * spec * matSpecular * intensity;

	vec3 result = (ambient + diffuse + specular) * attenuation;
	FragColor = vec4(result, 1.0);
}