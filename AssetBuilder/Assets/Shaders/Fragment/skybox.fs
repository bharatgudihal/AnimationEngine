#version 420 core

in vec3 texCoords;

out vec4 FragColor;

struct Material{
	samplerCube diffuse;
};

uniform Material material;

void main(){
	vec3 color = texture(material.diffuse, texCoords).rgb;

	//HDR tonemap and gamma correct
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}