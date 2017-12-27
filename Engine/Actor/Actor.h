#pragma once
#include <Engine/Math/Transform.h>

namespace Engine {

	namespace Graphics {
		class Mesh;
		class Shader;
		class Material;
	}

	class Actor {
	public:
		Actor(Graphics::Mesh* mesh);
		~Actor();
		void SetMaterial(Graphics::Material* material);
		void Draw(Graphics::Shader* shader);
		Math::Transform transform;
	private:
		Graphics::Mesh* mesh;
		Graphics::Material* material;
	};
}

#include "Actor_Inl.h"