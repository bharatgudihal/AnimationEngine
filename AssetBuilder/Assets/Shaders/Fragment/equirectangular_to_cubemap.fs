#version 420 core

in vec3 texCoords;

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
};

uniform Material material;

const vec2 invAtan = vec2(0.1591, 0.3183);


//Reference: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
vec2 ConvertToSpherical(vec3 oldUV){
	vec2 uv = vec2(atan(oldUV.z, oldUV.x), asin(oldUV.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main(){
	vec2 uv = ConvertToSpherical(normalize(texCoords));
	FragColor = vec4(texture(material.diffuse, uv).rgb, 1.0);
}