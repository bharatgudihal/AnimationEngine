#pragma once

#include <Engine/Utility/ReferenceCounted.h>
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		
		class Texture2D;
		class Shader;

		class Material {
		public:
			static Material* CreateMaterial(Texture2D* diffuseTexture, Texture2D* specularTexture, glm::vec3 diffuseColor = glm::vec3(0.0f), glm::vec3 specularColor = glm::vec3(0.0f), const float shininess = 32.0f);
			static void DestroyMaterial(Material* material);
			void Bind(Shader* shader);
			void SetDiffuseTexture(Texture2D* newTexture);
			void SetSpecularTexture(Texture2D* newTexture);
			void SetDiffuseColor(const glm::vec3 newColor);
			void SetSpecularColor(const glm::vec3 newColor);
			REFERENCE_COUNT_FUNCTIONS
		private:
			Material(Texture2D* diffuseTexture, Texture2D* specularTexture, const float shininess, glm::vec3 diffuseColor, glm::vec3 specularColor);
			~Material();
			Texture2D* diffuseTexture;
			Texture2D* specularTexture;
			glm::vec3 diffuseColor;
			glm::vec3 specularColor;
			float shininess;
			REFERENCE_COUNT_VARIABLES
		};
	}
}