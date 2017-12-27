#pragma once

#include <Engine/Lighting/Light.h>

namespace Engine {
	namespace Lighting {
		class DirectionalLight : public Light {
		public:
			DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor, glm::vec3 direction);
			~DirectionalLight();
			void Draw(Graphics::Shader* shader);
			glm::vec3 GetLightDirection() const;
		private:
			glm::vec3 lightDirection;
		};
	}
}

#include "DirectionalLight_Inl.h"