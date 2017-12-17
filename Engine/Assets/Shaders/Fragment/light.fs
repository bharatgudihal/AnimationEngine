#version 420 core

out vec4 FragColor;

in vec3 ObjectColor;

void main()
{
	FragColor = vec4(ObjectColor, 1.0);
}