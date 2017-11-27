#version 420 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform bool useTexture1;
uniform bool useTexture2;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendRatio;

void main()
{
	if(useTexture1 && useTexture2){
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendRatio);
	}else if(useTexture1){
		FragColor = texture(texture1, TexCoord);
	}else{
		FragColor = vec4(ourColor, 1.0);
	}
}