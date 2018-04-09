#include "EquirectangularMap.h"
#include <Engine/Utility/TextureLoader.h>
#include <glad/glad.h>
#include <iostream>

Engine::Graphics::EquirectangularMap::EquirectangularMap(const std::string& textureFile)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int width, height, channels;
	float* data = Utility::LoadHDRTexture(textureFile.c_str(), width, height, channels, true);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
	}
	else {
		std::cout << "Failed to load texture in path " << textureFile << std::endl;
	}
}

Engine::Graphics::EquirectangularMap::~EquirectangularMap()
{
	glDeleteTextures(1, &textureId);
}

