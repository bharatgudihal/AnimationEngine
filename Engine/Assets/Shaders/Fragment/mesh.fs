#version 420 core

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform bool useTexture1;
uniform bool useTexture2;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendRatio;
uniform Light light;
uniform vec3 viewPos;
uniform Material material;

void main()
{
	if(useTexture1 && useTexture2){
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendRatio);
	}else if(useTexture1){
		FragColor = texture(texture1, TexCoord);
	}else{
		//ambient
		vec3 ambient = light.ambient * material.ambient;

		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDirection = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = light.diffuse * (diff * material.diffuse);

		//specular
		vec3 viewDirection = normalize(viewPos - FragPos);
		vec3 reflectedDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
		vec3 specular = light.specular * (spec * material.specular);

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}
}