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
		Actor(Graphics::Mesh* mesh, Graphics::Material* material);
		~Actor();
		void Draw(Graphics::Shader* shader);
		Graphics::Material* GetMaterial(const unsigned int index);
		Graphics::Mesh* GetMesh(const unsigned int index);
		Math::Transform transform;
	private:
		std::vector<Graphics::Mesh*> meshes;
		std::vector<Graphics::Material*> materials;
	};
}