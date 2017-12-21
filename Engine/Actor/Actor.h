#pragma once
#include <Engine/Math/Transform.h>

namespace Engine {

	namespace Graphics {
		class Mesh;
		class Shader;
	}

	class Actor {
	public:
		Actor(Graphics::Mesh* mesh, Graphics::Shader* shader);
		~Actor();
		void Draw();
		Math::Transform transform;
	private:
		Graphics::Mesh* mesh;
		Graphics::Shader* shader;

	};
}