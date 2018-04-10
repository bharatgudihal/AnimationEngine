#version 420 core

in vec3 texCoords;

out vec4 FragColor;

struct Material{
	samplerCube diffuse;
};

uniform Material material;

const float PI = 22.0 / 7.0;

void main(){
	vec3 N = normalize(texCoords);
	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, N);
	up = cross(N, right);

	float sampleDelta = 0.025;
	float numberOfSamples = 0.0;
	//Discretely sample the cubemap over a hemisphere. Reference: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta){
		for(float theta = 0.0; theta < PI * 0.5; theta += sampleDelta){
			//convert from spherical to cartesian coordinates in tangent space
			vec3 sampleVector = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			//tangent to world space
			sampleVector = sampleVector.x * right + sampleVector.y * up + sampleVector.z * N;

			irradiance += texture(material.diffuse, sampleVector).rgb * cos(theta) * sin(theta);
			numberOfSamples++;
		}
	}

	irradiance = PI * irradiance / numberOfSamples;

	FragColor = vec4(irradiance, 1.0);
}