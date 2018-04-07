#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos * 2.0, 1.0);
	TexCoord = aTexCoord;
}