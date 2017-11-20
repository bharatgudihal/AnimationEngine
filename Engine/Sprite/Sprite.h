#pragma once
#include <Engine/Math/Transform.h>

namespace Engine {
	namespace Graphics {
		
		class Shader;
		class Texture;

		class Sprite {
		public:
			static Sprite* CreateSprite(const float* vertexArray, const unsigned int vertexCount, const unsigned int* indexArray, const unsigned int indexCount);
			void Draw();
			static void DestroySprite(Sprite* sprite);
			void SetTexture1(const char* textureFile);
			void SetTexture2(const char* textureFile, float blendRatio);

			Math::Transform transform;

		private:
			Sprite(const float* vertexArray, const unsigned int vertexCount, const unsigned int* indexArray, const unsigned int indexCount);
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
			unsigned int indexCount;
		};
	}
}