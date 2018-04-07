#version 420 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D hdrRender;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrRender, TexCoord).rgb;

	//exposure tone mapping
	vec3 toneMappedColor = vec3(1.0) - exp(-hdrColor * exposure);

	toneMappedColor = pow(toneMappedColor, vec3(1.0 / gamma));
	
	FragColor = vec4(toneMappedColor, 1.0);
}