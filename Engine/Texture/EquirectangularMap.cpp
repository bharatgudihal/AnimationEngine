#include "EquirectangularMap.h"
#include <Engine/Utility/TextureLoader.h>
#include <glad/glad.h>
#include <iostream>
#include <cassert>

Engine::Graphics::EquirectangularMap * Engine::Graphics::EquirectangularMap::CreateEquirectangularMap(const std::string & textureFile)
{
	return new EquirectangularMap(textureFile);
}

void Engine::Graphics::EquirectangularMap::DestroyEquirectangularMap(EquirectangularMap * equirectangularMap)
{
	assert(equirectangularMap);
	if (!equirectangularMap->DecrementReferenceCount()) {
		delete equirectangularMap;
	}
}

void Engine::Graphics::EquirectangularMap::SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterParam);
}

void Engine::Graphics::EquirectangularMap::SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrappingParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrappingParam);
}

void Engine::Graphics::EquirectangularMap::Bind(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

Engine::Graphics::EquirectangularMap::EquirectangularMap(const std::string& textureFile)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int width, height, channels;
	float* data = Utility::LoadHDRTexture(textureFile.c_str(), width, height, channels, true);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		Utility::FreeTexture(data);
	}
	else {
		std::cout << "Failed to load texture in path " << textureFile << std::endl;
	}
}

Engine::Graphics::EquirectangularMap::~EquirectangularMap()
{
	glDeleteTextures(1, &textureId);
}

