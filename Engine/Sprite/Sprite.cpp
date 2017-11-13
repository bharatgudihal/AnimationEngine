#include "Sprite.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>

Engine::Sprite* Engine::Sprite::CreateSprite(const float * vertexArray, const unsigned int vertexCount, const unsigned int * indexArray, const unsigned int indexCount)
{
	return new Sprite(vertexArray, vertexCount, indexArray, indexCount);
}

Engine::Sprite::Sprite(const float * vertexArray, const unsigned int vertexCount, const unsigned int * indexArray, const unsigned int indexCount)
{
	this->indexCount = indexCount;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the VAO, VBO, EBO and set attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertexArray, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indexArray, GL_STATIC_DRAW);

	// unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind VAO
	glBindVertexArray(0);
}

Engine::Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Engine::Sprite::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Engine::Sprite::DestroySprite(Sprite* sprite)
{
	delete sprite;
}
