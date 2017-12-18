#include "UniformBuffer.h"
#include <glad\glad.h>
#include <Engine/UniformBuffer/UniformBuffers.h>

Engine::Graphics::UniformBuffer::UniformBuffer(const UniformBufferType type, const unsigned int usage)
{
	switch (type) {
	case DataPerFrame:
		dataSize = sizeof(UniformBuffers::DataPerFrame);
	}
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, usage);
	glBindBufferBase(GL_UNIFORM_BUFFER, type, bufferId);
}

Engine::Graphics::UniformBuffer::~UniformBuffer()
{
}

void Engine::Graphics::UniformBuffer::Update(const void * data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);
}
