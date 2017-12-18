#pragma once

#include <Externals/glm/Includes.h>

namespace Engine {		

	class Actor;

	namespace Graphics {
		class Shader;
	}

	namespace Lighting {
		class SimpleLight {
		public:
			SimpleLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor* actor);
			~SimpleLight();
			void ShowMesh(const bool showMesh);
			void Draw();
			glm::vec3 GetPosition() const;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		private:			
			Actor* actor;
			bool showMesh;
			unsigned int bufferId;
		};
	}
}

#include "SimpleLight_Inl.h"