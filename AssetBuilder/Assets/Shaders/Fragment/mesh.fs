#version 420 core

struct LightData {	
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct DirectionalLight {
	vec4 direction;
				
	LightData lightData;
				
	vec4 isActive;
};

struct PointLight {
	vec4 position;
				
	LightData lightData;

	float linear;
	float quadratic;

	float isActive;

	float padding;
};

struct SpotLight {
	vec4 direction;
	vec4 position;

	LightData lightData;
				
	float innerCutOff;
	float outerCutOff;
				
	float isActive;

	float padding;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
	bool hasDiffuse;
	bool hasSpecular;
};

#define NR_POINT_LIGHTS 4
layout (std140, binding = 0) uniform dataPerFrame {
	mat4 view;
	mat4 projection;
	vec4 viewPos;
	
	DirectionalLight directionalLight;

	PointLight pointLights[NR_POINT_LIGHTS];

	SpotLight spotLight;

	float gamma;
	vec3 padding;
};

out vec4 FragColor;

in vec3 ObjectColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
	// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(vec3(viewPos) - FragPos);

	//Calculate directional lighting
	vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);

	//Calculate point lighting
	for(int i = 0; i < NR_POINT_LIGHTS ; i++){
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	//Calculate spot lightin
	result += CalcSpotLight(spotLight, FragPos, norm, viewDir);

	result = pow(result, vec3(1.0/gamma));

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	if(light.isActive.x == 1.0){
		vec3 lightDir = normalize(-vec3(light.direction));
    
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
    
		// specular shading
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
		// combine results
		vec3 ambient = vec3(light.lightData.ambient);
		vec3 diffuse = vec3(light.lightData.diffuse);
		if(material.hasDiffuse){
			ambient = ambient  * vec3(texture(material.diffuse, TexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, TexCoord));
		}else{
			specular = specular * spec * material.specularColor;
		}
    
		return (ambient + diffuse + specular);
	}else{
		return vec3(0.0);
	}
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(light.isActive == 1.0){
		vec3 lightPos = vec3(light.position);
	
		vec3 lightDir = normalize(lightPos - fragPos);
    
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
    
		// specular shading
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
		// attenuation
		float distance = length(lightPos - fragPos);
		float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));
    
		// combine results
		vec3 ambient = vec3(light.lightData.ambient);
		vec3 diffuse = vec3(light.lightData.diffuse);
		if(material.hasDiffuse){
			ambient = ambient  * vec3(texture(material.diffuse, TexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, TexCoord));
		}else{
			specular = specular * spec * material.specularColor;
		}
    
		return (ambient + diffuse + specular) * attenuation;
	}else{
		return vec3(0.0);
	}
}

vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir){
	if(light.isActive == 1.0){
		vec3 lightPos = vec3(light.position);
	
		vec3 lightDir = normalize(lightPos - fragPos);

		float theta = dot(lightDir, normalize(-vec3(light.direction)));
		float epsilon = light.innerCutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
    
		// specular shading
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

		// combine results
		vec3 ambient = vec3(light.lightData.ambient);
		vec3 diffuse = vec3(light.lightData.diffuse);
		if(material.hasDiffuse){
			ambient = ambient  * vec3(texture(material.diffuse, TexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, TexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, TexCoord));
		}else{
			specular = specular * spec * material.specularColor;
		}

		ambient *= intensity;
		diffuse *= intensity;
		specular *= intensity;
    
		return (ambient + diffuse + specular);

	}else{
		return vec3(0.0);
	}
}