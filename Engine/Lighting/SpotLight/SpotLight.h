#pragma once

#include <Engine/Lighting/Light.h>

namespace Engine {
	namespace Lighting {
		class SpotLight :public Light {
		public:
			SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor, glm::vec3 direction, float innerCutOff, float outerCutOff);
			~SpotLight();
			void Draw();
			void SetInnerCutOffDegrees(float cutOffDegrees);
			void SetInnerCutOff(float cutOff);
			float GetInnerCutOff() const;
			void SetOuterCutOffDegrees(float cutOffDegrees);
			void SetOuterCutOff(float cutOff);
			float GetOuterCutOff() const;
			void SetDirection(glm::vec3 direction);
			glm::vec3 GetDirection() const;
		private:
			glm::vec3 direction;
			float innerCutOff;
			float outerCutOff;
		};
	}
}

#include "SpotLight_Inl.h"