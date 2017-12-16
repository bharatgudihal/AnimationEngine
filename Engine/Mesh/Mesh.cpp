#include "Mesh.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine\Texture\Texture.h>
#include <Engine\Shader\Shader.h>
#include <Engine\Math\Math.h>
#include <Engine\Camera\Camera.h>

Engine::Graphics::Mesh* Engine::Graphics::Mesh::CreateMesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount)
{
	return new Mesh(vertexArray, vertexCount, indexArray, indexCount);
}

Engine::Graphics::Mesh * Engine::Graphics::Mesh::GetCube()
{
	Engine::Graphics::VertexFormat::Mesh meshVertexData[] = {
		// positions			// colors				// texture coords
		//Back face
		-0.5f, -0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		1.0f, 1.0f,

		//Front face
		-0.5f, -0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 1.0f,

		//Left face
		-0.5f,  0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		1.0f, 0.0f,

		//Right face
		0.5f,  0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		0.0f, 0.0f,

		//Bottom face
		-0.5f, -0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 1.0f,

		//Top face
		-0.5f,  0.5f, -0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 0.5f, 0.31f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.5f, 0.31f,		0.0f, 0.0f
	};

	//index data
	uint32_t indices[] = {
		0, 2, 1, 0, 3, 2,
		4, 5, 6, 4, 6, 7,
		10, 11, 8, 10, 8, 9,
		15, 14, 13, 15, 13, 12,
		16, 17, 18, 16, 18, 19,
		23, 22, 21, 23, 21, 20
	};
	Mesh* mesh = CreateMesh(meshVertexData, sizeof(meshVertexData) / sizeof(Engine::Graphics::VertexFormat::Mesh), indices, sizeof(indices) / sizeof(uint32_t));	
	return mesh;
}

Engine::Graphics::Mesh::Mesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount)
{
	this->indexCount = indexCount;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the VAO, VBO, EBO and set attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat::Mesh) * vertexCount, vertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, indexArray, GL_STATIC_DRAW);

	//position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute (3 floats)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//UV attribute (2 floats)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Engine::Graphics::Mesh::~Mesh()
{	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Engine::Graphics::Mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Engine::Graphics::Mesh::DestroyMesh(Mesh* Mesh)
{
	delete Mesh;
}