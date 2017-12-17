#version 420 core

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform bool useTexture1;
uniform bool useTexture2;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendRatio;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1;
	float specularStrength = 0.5;
	float shininess = 32;

	if(useTexture1 && useTexture2){
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendRatio);
	}else if(useTexture1){
		FragColor = texture(texture1, TexCoord);
	}else{	

		vec3 ambient = ambientStrength * lightColor;

		vec3 norm = normalize(Normal);
		vec3 lightDirection = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = diff * lightColor;

		vec3 viewDirection = normalize(viewPos - FragPos);
		vec3 reflectedDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), shininess);
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * ObjectColor;		
		FragColor = vec4(result, 1.0);
	}
}