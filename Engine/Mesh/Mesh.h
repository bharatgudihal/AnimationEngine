#pragma once
#include <Engine/Math/Transform.h>
#include <Engine/VertexFormats.h>
#include <stdint.h>

namespace Engine {
	namespace Graphics {

		class Shader;
		class Texture;
		class Camera;

		class Mesh {
		public:
			static Mesh* CreateMesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			static Mesh* GetCube();
			void Draw(const Camera* camera);
			static void DestroyMesh(Mesh* mesh);
			void SetShaders(const char* vertexShaderFile, const char* fragmentShader);
			void SetTexture1(const char* textureFile);
			void SetTexture2(const char* textureFile, float blendRatio);

			Math::Transform transform;

		private:
			Mesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount);
			Mesh();
			Mesh(Mesh& other);
			~Mesh();

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