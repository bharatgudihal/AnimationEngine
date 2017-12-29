#pragma once
#include <Engine/Math/Transform.h>
#include <vector>

namespace Engine {

	namespace Graphics {
		class Mesh;
		class Shader;
		class Material;
	}

	class Actor {
	public:
		Actor(std::vector<Graphics::Mesh*> meshes, std::vector<Graphics::Material*> materials);
		~Actor();
		void Draw(Graphics::Shader* shader);
		Math::Transform transform;
	private:
		std::vector<Graphics::Mesh*> meshes;
		std::vector<Graphics::Material*> materials;
	};
}