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

void main()
{
	float ambientStrength = 0.1;
	if(useTexture1 && useTexture2){
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendRatio);
	}else if(useTexture1){
		FragColor = texture(texture1, TexCoord);
	}else{
		vec3 norm = normalize(Normal);
		vec3 lightDirection = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = diff * lightColor;

		vec3 ambient = ambientStrength * lightColor;
		vec3 result = (ambient + diffuse) * ObjectColor;		
		FragColor = vec4(result, 1.0);
	}
}