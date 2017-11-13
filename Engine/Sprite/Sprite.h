#pragma once

namespace Engine {
	class Sprite {
	public:
		static Sprite* CreateSprite(const float* vertexArray, const unsigned int vertexCount, const unsigned int* indexArray, const unsigned int indexCount);
		void Draw();
		static void DestroySprite(Sprite* sprite);
	private:
		Sprite(const float* vertexArray, const unsigned int vertexCount, const unsigned int* indexArray, const unsigned int indexCount);		
		Sprite();
		Sprite(Sprite& other);
		~Sprite();

		//Vertex Buffer Object, Vertex Array Object and Element Buffer Object
		unsigned int VBO, VAO, EBO;
		unsigned int indexCount;
	};
}