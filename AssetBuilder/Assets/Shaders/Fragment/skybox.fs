#version 420 core

in vec3 texCoords;

out vec4 FragColor;

struct Material{
	samplerCube diffuse;
};

uniform Material material;

void main(){
	FragColor = texture(material.diffuse, texCoords);
}