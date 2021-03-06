#pragma once
#include <Engine/Math/Transform.h>
#include <Engine/VertexFormats.h>
#include <Engine/Utility/ReferenceCounted.h>
#include <stdint.h>

namespace Engine {
	namespace Graphics {

		class Mesh {
		public:
			static Mesh* CreateMesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			static Mesh* GetCube();
			static Mesh* GetCube(const float r, const float g, const float b);
			static Mesh* GetPlane();
			static Mesh* GetPlane(const float r, const float g, const float b);
			void Draw();
			static void DestroyMesh(Mesh* mesh);
			REFERENCE_COUNT_FUNCTIONS

		private:
			Mesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			Mesh();
			Mesh(Mesh& other);
			Mesh& operator=(Mesh& other);
			~Mesh();
			static void CalculateTangentsAndBitangents(VertexFormat::Mesh* vertexArray, const uint32_t* indexArray, const uint32_t indexCount);		

			//Vertex Buffer Object, Vertex Array Object and Element Buffer Object
			unsigned int VBO, VAO, EBO;
			uint32_t indexCount;
			REFERENCE_COUNT_VARIABLES
		};
	}
}