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

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	blendRatio = 1.0f;

	shader = new Shader("Assets/Shaders/mesh.vs", "Assets/Shaders/mesh.fs");
	shader->Use();
	shader->SetFloat("blendRatio", blendRatio);
}

Engine::Graphics::Mesh::~Mesh()
{
	delete shader;

	if (texture1) {
		Texture::DestroyTexture(texture1);
	}

	if (texture2) {
		Texture::DestroyTexture(texture2);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Engine::Graphics::Mesh::Draw(const Camera* camera)
{
	if (texture1) {
		texture1->Bind();
	}

	if (texture2) {
		texture2->Bind();
	}

	shader->Use();
	shader->SetMatrix("model", Math::CalculateTransform(transform));
	shader->SetMatrix("view", camera->GetViewMatrix());
	shader->SetMatrix("projection", camera->GetProjectionMatrix());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Engine::Graphics::Mesh::DestroyMesh(Mesh* Mesh)
{
	delete Mesh;
}

void Engine::Graphics::Mesh::SetTexture1(const char * textureFile)
{
	if (texture1) {
		Texture::DestroyTexture(texture1);
	}
	texture1 = Texture::CreateTexture(textureFile, 0);
	shader->Use();
	shader->SetInt("texture1", 0);
}

void Engine::Graphics::Mesh::SetTexture2(const char * textureFile, float blendRatio)
{
	if (texture2) {
		Texture::DestroyTexture(texture2);
	}
	texture2 = Texture::CreateTexture(textureFile, 1);
	this->blendRatio = blendRatio;
	shader->Use();
	shader->SetInt("texture2", 1);
	shader->SetFloat("blendRatio", blendRatio);
}
