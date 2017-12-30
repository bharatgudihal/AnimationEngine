#pragma once

#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		
		class Texture;
		class Shader;

		class Material {
		public:
			static Material* CreateMaterial(Texture* diffuseTexture, Texture* specularTexture, const float shininess = 32.0f);
			static void DestroyMaterial(Material* material);
			void Bind(Shader* shader);
			REFERENCE_COUNT_FUNCTIONS
		private:
			Material(Texture* diffuseTexture, Texture* specularTexture, const float shininess);
			~Material();
			Texture* diffuseTexture;
			Texture* specularTexture;
			float shininess;
			REFERENCE_COUNT_VARIABLES
		};
	}
}