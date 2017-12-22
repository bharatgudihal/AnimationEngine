#pragma once

#include <Engine/Lighting/Light.h>

namespace Engine {
	namespace Lighting {
		class SpotLight :public Light {
		public:
			SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor, glm::vec3 direction, float cutOff);
			~SpotLight();
			void Draw();
			void SetCutOffDegrees(float cutOffDegrees);
			void SetCutOff(float cutOff);
			float GetCutOff() const;
			void SetDirection(glm::vec3 direction);
			glm::vec3 GetDirection() const;
		private:
			glm::vec3 direction;
			float cutOff;
		};
	}
}

#include "SpotLight_Inl.h"