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
	sampler2D normalMap;
	sampler2D depthMap;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
	float heightScale;
	bool hasDiffuse;
	bool hasSpecular;
	bool hasNormalMap;
	bool hasDepthMap;
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
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform Material material;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir);
vec2 DoParallaxMapping(vec2 UV, vec3 tangentSpaceViewDir);

vec2 correctedTexCoord;

void main()
{
	//Get tangent space viewDirection
	vec3 tangentSpaceViewDir = normalize(TangentViewPos - TangentFragPos);
	
	if(material.hasDepthMap){
		correctedTexCoord = DoParallaxMapping(TexCoord, tangentSpaceViewDir);
		if(correctedTexCoord.x > 1.0 || correctedTexCoord.x < 0.0 || correctedTexCoord.y > 1.0 || correctedTexCoord.y < 0.0){
			discard;
		}
	}else{
		correctedTexCoord = TexCoord;
	}

	// properties
    vec3 norm;
	if(material.hasNormalMap){
		//sample normal map
		norm = texture(material.normalMap, correctedTexCoord).rgb;
		
		//change normal range from [0,1] to [-1,1]
		norm = normalize(norm * 2.0 - 1.0);

		//Transform normal from tangent space
		norm = normalize(TBN * norm);
	}else{
		norm = normalize(Normal);
	}

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

vec2 DoParallaxMapping(vec2 UV, vec3 tangentSpaceViewDir){
	//Steep parallax mapping
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numberOfLayers = mix(minLayers, maxLayers, abs(dot(vec3(0.0,0.0,1.0), tangentSpaceViewDir)));
	float depthPerLayer = 1.0 / numberOfLayers;
	vec2 uvOffset = tangentSpaceViewDir.xy / tangentSpaceViewDir.z * material.heightScale;
	vec2 uvOffsetPerLayer = uvOffset / numberOfLayers;
	float currentDepth = 0.0;
	vec2 currentUV = UV;
	float currentDepthMapValue = texture(material.depthMap, currentUV).r;

	while(currentDepth < currentDepthMapValue){
		currentUV -= uvOffsetPerLayer;
		currentDepthMapValue = texture(material.depthMap, currentUV).r;
		currentDepth += depthPerLayer;
	}

	//Parallax occlusion
	vec2 previousUV = currentUV + uvOffsetPerLayer;
	float currentDepthDiff = currentDepthMapValue - currentDepth;
	float previousDepthDifference = texture(material.depthMap, previousUV).r - currentDepth + depthPerLayer;
	float weight = currentDepthDiff / (currentDepthDiff - previousDepthDifference);
	vec2 finalUV = previousUV * weight + currentUV * (1 - weight);

	return finalUV;
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
			ambient = ambient  * vec3(texture(material.diffuse, correctedTexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, correctedTexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, correctedTexCoord));
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
			ambient = ambient  * vec3(texture(material.diffuse, correctedTexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, correctedTexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, correctedTexCoord));
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
			ambient = ambient  * vec3(texture(material.diffuse, correctedTexCoord));
			diffuse = diffuse  * diff * vec3(texture(material.diffuse, correctedTexCoord));
		}else{
			ambient = ambient * material.diffuseColor;
			diffuse = diffuse * diff * material.diffuseColor;
		}

		vec3 specular = vec3(light.lightData.specular);
		if(material.hasSpecular){
			specular = specular * spec * vec3(texture(material.specular, correctedTexCoord));
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