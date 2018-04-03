#include "Mesh.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine\Texture\Texture2D.h>
#include <Engine\Shader\Shader.h>
#include <Engine\Math\Math.h>
#include <Engine\Camera\Camera.h>

Engine::Graphics::Mesh* Engine::Graphics::Mesh::CreateMesh(const VertexFormat::Mesh* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount)
{
	return new Mesh(vertexArray, vertexCount, indexArray, indexCount);
}

Engine::Graphics::Mesh * Engine::Graphics::Mesh::GetCube(const float r, const float g, const float b) {
	Engine::Graphics::VertexFormat::Mesh meshVertexData[] = {
		// positions			// colors		// texture coords	//Normals			//Tangent			//Bitangent
		//Back face
		-0.5f, -0.5f, -0.5f,	r, g, b,		0.0f, 0.0f,			0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		r, g, b,		1.0f, 0.0f,			0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		r, g, b,		1.0f, 1.0f,			0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	r, g, b,		0.0f, 1.0f,			0.0f, 0.0f, -1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

		//Front face
		-0.5f, -0.5f,  0.5f,	r, g, b,		0.0f, 0.0f,			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		r, g, b,		1.0f, 0.0f,			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		r, g, b,		1.0f, 1.0f,			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	r, g, b,		0.0f, 1.0f,			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

		//Left face
		-0.5f,  0.5f,  0.5f,	r, g, b,		0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	r, g, b,		1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	r, g, b,		1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	r, g, b,		0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

		//Right face
		0.5f,  0.5f,  0.5f,		r, g, b,		0.0f, 0.0f,			1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		r, g, b,		1.0f, 0.0f,			1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		r, g, b,		1.0f, 1.0f,			1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		r, g, b,		0.0f, 1.0f,			1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

		//Bottom face
		-0.5f, -0.5f, -0.5f,	r, g, b,		0.0f, 0.0f,			0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		r, g, b,		1.0f, 0.0f,			0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		r, g, b,		1.0f, 1.0f,			0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	r, g, b,		0.0f, 1.0f,			0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

		//Top face
		-0.5f,  0.5f, -0.5f,	r, g, b,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		r, g, b,		1.0f, 0.0f,			0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		r, g, b,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	r, g, b,		0.0f, 1.0f,			0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
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

Engine::Graphics::Mesh * Engine::Graphics::Mesh::GetCube()
{	
	return GetCube(1.0f,1.0f,1.0f);
}

Engine::Graphics::Mesh * Engine::Graphics::Mesh::GetPlane(const float r, const float g, const float b)
{
	Engine::Graphics::VertexFormat::Mesh meshVertexData[] = {
		// positions			// colors		// texture coords	//Normals			//Tangent			//Bitangent
		-0.5f,  0.0f, -0.5f,	r, g, b,		0.0f, 1.0f,			0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		0.5f,  0.0f, -0.5f,		r, g, b,		1.0f, 1.0f,			0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		0.5f,  0.0f,  0.5f,		r, g, b,		1.0f, 0.0f,			0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-0.5f,  0.0f,  0.5f,	r, g, b,		0.0f, 0.0f,			0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f
	};

	//index data
	uint32_t indices[] = {
		0, 2, 1, 0, 3, 2
	};

	size_t indexCount = sizeof(indices) / sizeof(uint32_t);

	CalculateTangentsAndBitangents(meshVertexData, indices, indexCount);

	Mesh* mesh = CreateMesh(meshVertexData, sizeof(meshVertexData) / sizeof(Engine::Graphics::VertexFormat::Mesh), indices, sizeof(indices) / sizeof(uint32_t));
	return mesh;
}

Engine::Graphics::Mesh * Engine::Graphics::Mesh::GetPlane()
{
	return GetPlane(1.0f, 1.0f, 1.0f);
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

	uint8_t stride = sizeof(VertexFormat::Mesh);

	//position attribute (3 floats)
	//offset = 0 floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute (3 floats)
	//offset = 3 floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexFormat::Mesh, color));
	glEnableVertexAttribArray(1);	

	//Normal attribute (3 floats)
	//offset = 6 floats
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexFormat::Mesh, normal));
	glEnableVertexAttribArray(2);

	//UV attribute (2 floats)
	//offset = 9 floats
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexFormat::Mesh, UV));
	glEnableVertexAttribArray(3);
}

Engine::Graphics::Mesh::~Mesh()
{	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

static void CalculateTangentsAndBitangentsForSingleVertex(Engine::Graphics::VertexFormat::Mesh * vertexArray, uint32_t index1, uint32_t index2, uint32_t index3)
{
	glm::vec3 edge1 = glm::vec3(vertexArray[index2].position.x, vertexArray[index2].position.y, vertexArray[index2].position.z) - glm::vec3(vertexArray[index1].position.x, vertexArray[index1].position.y, vertexArray[index1].position.z);
	glm::vec3 edge2 = glm::vec3(vertexArray[index3].position.x, vertexArray[index3].position.y, vertexArray[index3].position.z) - glm::vec3(vertexArray[index1].position.x, vertexArray[index1].position.y, vertexArray[index1].position.z);
	glm::vec2 deltaUV1 = glm::vec2(vertexArray[index2].UV.u, vertexArray[index2].UV.v) - glm::vec2(vertexArray[index1].UV.u, vertexArray[index1].UV.v);
	glm::vec2 deltaUV2 = glm::vec2(vertexArray[index3].UV.u, vertexArray[index3].UV.v) - glm::vec2(vertexArray[index1].UV.u, vertexArray[index1].UV.v);

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	glm::vec3 bitangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);

	vertexArray[index1].tangent.x = tangent.x;
	vertexArray[index1].tangent.y = tangent.y;
	vertexArray[index1].tangent.z = tangent.z;
	vertexArray[index1].bitangent.x = bitangent.x;
	vertexArray[index1].bitangent.y = bitangent.y;
	vertexArray[index1].bitangent.z = bitangent.z;
}

void Engine::Graphics::Mesh::CalculateTangentsAndBitangents(VertexFormat::Mesh * vertexArray, const uint32_t * indexArray, const uint32_t indexCount)
{
	for (int i = 0; i < indexCount; i+=3) {
		uint32_t index1, index2, index3;
		index1 = indexArray[i];
		index2 = indexArray[i + 1];
		index3 = indexArray[i + 2];
		
		CalculateTangentsAndBitangentsForSingleVertex(vertexArray, index1, index2, index3);
		CalculateTangentsAndBitangentsForSingleVertex(vertexArray, index2, index3, index1);
		CalculateTangentsAndBitangentsForSingleVertex(vertexArray, index3, index1, index2);
	}
}

void Engine::Graphics::Mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Engine::Graphics::Mesh::DestroyMesh(Mesh* mesh)
{
	if (!mesh->DecrementReferenceCount()) {
		delete mesh;
	}
}