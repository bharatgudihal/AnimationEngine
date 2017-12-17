#pragma once

#include <Engine/Math/Transform.h>

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
			void Apply(Graphics::Shader* shader);
			void ShowMesh(const bool showMesh);
			void Draw();					
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		private:			
			Actor* actor;
			bool showMesh;
		};
	}
}