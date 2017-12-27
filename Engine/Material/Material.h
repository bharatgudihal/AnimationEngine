#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		class Texture;
		class Shader;

		class Material {
		public:
			Material(Texture* diffuseTexture, Texture* specularTexture, const float shininess);
			void Bind(Shader* shader);
			~Material();
		private:
			Texture* diffuseTexture;
			Texture* specularTexture;
			float shininess;
		};
	}
}