#pragma once

#include <Externals/glm/Includes.h>

namespace Engine {
	class Actor;
	namespace Lighting {
		class Light {
		public:
			Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor);
			void ShowMesh(const bool showMesh);			
			void SetPosition(glm::vec3 position);
			glm::vec3 GetPosition() const;
			virtual void Draw() = 0;
			virtual ~Light() {};
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		protected:
			Actor* actor;
			bool showMesh;
			unsigned int bufferId;
		};
	}
}

#include "Light_Inl.h"