#pragma once
#include <Engine/Math/Transform.h>
#include <Engine/VertexFormats.h>
#include <stdint.h>

namespace Engine {
	namespace Graphics {

		class Mesh {
		public:
			static Mesh* CreateMesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			static Mesh* GetCube();
			void Draw();
			static void DestroyMesh(Mesh* mesh);

		private:
			Mesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			Mesh();
			Mesh(Mesh& other);
			~Mesh();

			//Vertex Buffer Object, Vertex Array Object and Element Buffer Object
			unsigned int VBO, VAO, EBO;
			uint32_t indexCount;
		};
	}
}