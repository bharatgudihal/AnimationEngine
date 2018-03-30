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
			void SetDiffuseColor(const glm::vec3 newColor);
			void SetSpecularColor(const glm::vec3 newColor);
			REFERENCE_COUNT_FUNCTIONS
		private:
			Material(Texture* diffuseTexture, Texture* specularTexture, const float shininess, glm::vec3 diffuseColor, glm::vec3 specularColor);
			Material(Material& other);
			~Material();
			Material& operator=(Material& other);
			Texture* diffuseTexture;
			Texture* specularTexture;
			glm::vec3 diffuseColor;
			glm::vec3 specularColor;
			float shininess;
			REFERENCE_COUNT_VARIABLES
		};
	}
}