#pragma once

#include <Engine/Math/Transform.h>

namespace Engine {
	namespace Graphics {

		class Mesh;

		namespace Lighting {
			class SimpleLight {
			public:
				SimpleLight(glm::vec3 color);
				~SimpleLight();
				void ShowMesh(const bool showMesh);
				void Draw();
				Math::Transform transform;
			private:
				glm::vec3 color;
				Mesh* mesh;
				bool showMesh;
			};
		}
	}
}