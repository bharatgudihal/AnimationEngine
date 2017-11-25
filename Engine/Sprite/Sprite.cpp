#include "Sprite.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine\Texture\Texture.h>
#include <Engine\Shader\Shader.h>
#include <Engine\Math\Math.h>

Engine::Graphics::Sprite* Engine::Graphics::Sprite::CreateSprite(const float width, const float height, const VertexFormat::Color color)
{

	VertexFormat::Sprite vertexArray[4];
	
	//Top Left
	vertexArray[0].position.x = -width / 2.0f;
	vertexArray[0].position.y = height / 2.0f;
	vertexArray[0].position.z = 0.0f;
	vertexArray[0].color = { color.r,color.g,color.b };
	vertexArray[0].UV = { 0.0f,1.0f };

	//Bottom Left
	vertexArray[1].position.x = -width / 2.0f;
	vertexArray[1].position.y = -height / 2.0f;
	vertexArray[1].position.z = 0.0f;
	vertexArray[1].color = { color.r,color.g,color.b };
	vertexArray[1].UV = { 0.0f,0.0f };

	//Top Right
	vertexArray[2].position.x = width / 2.0f;
	vertexArray[2].position.y = height / 2.0f;
	vertexArray[2].position.z = 0.0f;
	vertexArray[2].color = { color.r,color.g,color.b };
	vertexArray[2].UV = { 1.0f,1.0f };

	//Bottom Right
	vertexArray[3].position.x = width / 2.0f;
	vertexArray[3].position.y = -height / 2.0f;
	vertexArray[3].position.z = 0.0f;
	vertexArray[3].color = { color.r,color.g,color.b };
	vertexArray[3].UV = { 1.0f,0.0f };

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 1, 3  // second triangle
	};

	return new Sprite(vertexArray, 4, indices, 6);
}

Engine::Graphics::Sprite::Sprite(const VertexFormat::Sprite* vertexArray, const uint32_t vertexCount, const uint32_t* indexArray, const uint32_t indexCount)
{
	this->indexCount = indexCount;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the VAO, VBO, EBO and set attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat::Sprite) * vertexCount, vertexArray, GL_STATIC_DRAW);

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
