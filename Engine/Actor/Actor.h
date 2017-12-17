#pragma once
#include <Engine/Math/Transform.h>

namespace Engine {

	namespace Graphics {
		class Mesh;
		class Shader;
		class Texture;
	}

	class Actor {
	public:
		Actor(Graphics::Mesh* mesh, Graphics::Shader* shader);
		~Actor();
		void Draw();
		void SetTexture1(Graphics::Texture* texture);
		void SetTexture2(Graphics::Texture* texture);
		Math::Transform transform;
	private:
		Graphics::Mesh* mesh;
		Graphics::Shader* shader;
		Graphics::Texture* texture1;
		Graphics::Texture* texture2;		

	};
}