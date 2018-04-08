#pragma once

#include <Engine/Utility/ReferenceCounted.h>
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		
		class Texture;
		class Shader;

		class Material {
		public:
			static Material* CreateMaterial(Texture* diffuseTexture, Texture* specularTexture, glm::vec3 diffuseColor = glm::vec3(0.0f), glm::vec3 specularColor = glm::vec3(0.0f), const float shininess = 32.0f);
			static void DestroyMaterial(Material* material);
			void Bind(Shader* shader);
			void SetDiffuseTexture(Texture* newTexture);
			void SetSpecularTexture(Texture* newTexture);			
			void SetNormalMap(Texture* newTexture);
			void SetDepthMap(Texture* newTexture, const float heightScale);
			void SetAlbedoMap(Texture* newTexture);
			void SetMetallicMap(Texture* newTexture);
			void SetRoughnessMap(Texture* newTexture);
			void SetAmbientOcclusionMap(Texture* newTexture);
			void SetDiffuseColor(const glm::vec3 newColor);
			void SetSpecularColor(const glm::vec3 newColor);
			void SetAlbedoColor(const glm::vec3 albedoColor);
			void SetMetalness(const float metalness);
			void SetRoughness(const float roughness);
			void SetAmbientOcclusion(const float ao);
			REFERENCE_COUNT_FUNCTIONS
		private:
			Material(Texture* diffuseTexture, Texture* specularTexture, const float shininess, glm::vec3 diffuseColor, glm::vec3 specularColor);
			Material(Material& other);
			~Material();
			Material& operator=(Material& other);
			Texture* diffuseTexture;
			Texture* specularTexture;
			Texture* normalMap;
			Texture* depthMap;
			Texture* albedoMap;
			Texture* metallicMap;
			Texture* roughnessMap;
			Texture* ambientOcclusionMap;
			glm::vec3 diffuseColor;
			glm::vec3 specularColor;
			glm::vec3 albedoColor;
			float shininess;
			float heightScale;
			float metalness;
			float roughness;
			float ao;
			REFERENCE_COUNT_VARIABLES
		};
	}
}

#include "Material_Inl.h"