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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform bool useTexture1;
uniform bool useTexture2;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendRatio;
uniform Material material;

void main()
{
	if(useTexture1 && useTexture2){
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendRatio);
	}else if(useTexture1){
		FragColor = texture(texture1, TexCoord);
	}else{
		//ambient
		vec3 ambient = lightAmbient * material.ambient;

		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDirection = normalize(lightPosition - FragPos);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = lightDiffuse * (diff * material.diffuse);

		//specular
		vec3 viewDirection = normalize(viewPos - FragPos);
		vec3 reflectedDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);
		vec3 specular = lightSpecular * (spec * material.specular);

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}
}