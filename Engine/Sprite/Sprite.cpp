#include "Sprite.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine\Texture\Texture.h>
#include <Engine\Shader\Shader.h>
#include <Engine\Math\Math.h>

Engine::Graphics::Sprite* Engine::Graphics::Sprite::CreateSprite(const float * vertexArray, const unsigned int vertexCount, const unsigned int * indexArray, const unsigned int indexCount)
{
	return new Sprite(vertexArray, vertexCount, indexArray, indexCount);
}

Engine::Graphics::Sprite::Sprite(const float * vertexArray, const unsigned int vertexCount, const unsigned int * indexArray, const unsigned int indexCount)
{
	this->indexCount = indexCount;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the VAO, VBO, EBO and set attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indexArray, GL_STATIC_DRAW);

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

	shader = new Shader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");
	shader->Use();
	shader->SetFloat("blendRatio", blendRatio);
}

Engine::Graphics::Sprite::~Sprite()
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

void Engine::Graphics::Sprite::Draw()
{
	if (texture1) {
		texture1->Bind();
	}
	
	if (texture2) {
		texture2->Bind();
	}

	shader->Use();
	shader->SetMatrix("transform", Math::CalculateTransform(transform));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Engine::Graphics::Sprite::DestroySprite(Sprite* sprite)
{
	delete sprite;
}

void Engine::Graphics::Sprite::SetTexture1(const char * textureFile)
{
	if (texture1) {
		Texture::DestroyTexture(texture1);
	}
	texture1 = Texture::CreateTexture(textureFile, 0);
	shader->Use();
	shader->SetInt("texture1", 0);
}

void Engine::Graphics::Sprite::SetTexture2(const char * textureFile, float blendRatio)
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
