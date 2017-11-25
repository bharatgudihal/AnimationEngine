#pragma once
#include <Engine/Math/Transform.h>
#include <Engine/VertexFormats.h>
#include <stdint.h>

namespace Engine {
	namespace Graphics {
		
		class Shader;
		class Texture;

		class Sprite {
		public:
			static Sprite* CreateSprite(const float width, const float height, const VertexFormat::Color color = {0.0f,0.0f,0.0f,0.0f});
			void Draw();
			static void DestroySprite(Sprite* sprite);
			void SetTexture1(const char* textureFile);
			void SetTexture2(const char* textureFile, float blendRatio);

			Math::Transform transform;

		private:
			Sprite(const VertexFormat::Sprite* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			Sprite();
			Sprite(Sprite& other);
			~Sprite();

			//Shader
			Shader* shader;

			//Textures
			Texture* texture1;
			Texture* texture2;
			float blendRatio;

			//Vertex Buffer Object, Vertex Array Object and Element Buffer Object
			unsigned int VBO, VAO, EBO;
			uint32_t indexCount;
		};
	}
}