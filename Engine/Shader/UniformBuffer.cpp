#include "UniformBuffer.h"

#include <glad/glad.h>

Engine::Graphics::UniformBuffer * Engine::Graphics::UniformBuffer::CreateUniformBuffer(const UniformBuffers::UniformBufferTypes type, const void* data)
{
	return new UniformBuffer(type, data);
}

void Engine::Graphics::UniformBuffer::DestroyUniformBuffer(UniformBuffer * uniformBuffer)
{
	delete uniformBuffer;
}

void Engine::Graphics::UniformBuffer::Update(const void * data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Engine::Graphics::UniformBuffer::UniformBuffer(const UniformBuffers::UniformBufferTypes type, const void* data):type(type) {
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);

	switch (type)
	{
	case UniformBuffers::UniformBufferTypes::DATA_PER_FRAME:
		dataSize = sizeof(UniformBuffers::dataPerFrame);
		break;
	}

	glBufferData(bufferId, dataSize, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(type), bufferId);
	//glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(type), bufferId, 0, dataSize);
}

Engine::Graphics::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &bufferId);
}
