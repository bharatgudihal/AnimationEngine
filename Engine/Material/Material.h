#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		class Texture;
		struct Material {
			Graphics::Texture* diffuse;
			glm::vec3 specular;
			float shininess;
		};
	}
}