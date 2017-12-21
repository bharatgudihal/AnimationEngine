#pragma once

#include <Engine/Lighting/Light.h>

namespace Engine {		

	class Actor;

	namespace Lighting {
		class PointLight : public Light {
		public:
			PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor);
			~PointLight();
			void Draw();
		};
	}
}