#pragma once

#include <Engine/Lighting/Light.h>
#include <Engine/Lighting/Attenuation.h>

namespace Engine {		

	class Actor;

	namespace Lighting {
		class PointLight : public Light {
		public:
			PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor, Attenuation& attenuation);
			~PointLight();
			void Draw(Graphics::Shader* shader);
			void SetAttenuation(Attenuation& attenuation);
			Attenuation GetAttenuation() const;
		private:
			Attenuation attenuation;
		};
	}
}

#include "PointLight_Inl.h"