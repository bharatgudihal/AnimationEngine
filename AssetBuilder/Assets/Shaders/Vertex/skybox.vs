#version 420 core

#define NR_POINT_LIGHTS 4
layout (std140, binding = 0) uniform dataPerFrame {
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	
	DirectionalLight directionalLight;

	PointLight pointLights[NR_POINT_LIGHTS];

	SpotLight spotLight;
};

layout (location = 0) vec3 vertexPosition;

out vec3 texCoords;

void main(){
	texCoords = vertexPosition;
	gl_Position = projection * mat4(mat3(view)) * vec4(vertexPosition, 1.0);
}