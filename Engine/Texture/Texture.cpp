#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <Externals/stb_image/Includes.h>

Engine::Graphics::Texture::Texture(const char * textureFileName) {

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);	
	
	//Load texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName, &width, &height, &nrChannels, 0);
	unsigned int pixelFormat = GL_RGB;
	if (nrChannels == 4) {
		pixelFormat = GL_RGBA;
	}
	if (data) {		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);
	}	
}

Engine::Graphics::Texture * Engine::Graphics::Texture::CreateTexture(const char * textureFileName)
{
	return new Texture(textureFileName);
}

void Engine::Graphics::Texture::DestroyTexture(Texture * texture)
{
	if (!texture->DecrementReferenceCount()) {
		delete texture;
	}
}

void Engine::Graphics::Texture::Bind(const unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Engine::Graphics::Texture::SetTextureFilteringParams(unsigned int minFilterParam, unsigned int maxFilterParam)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilterParam);
}

void Engine::Graphics::Texture::SetTextureWrappingParams(unsigned int sWrappingParam, unsigned int tWrappingParam)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrappingParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrappingParam);
}

Engine::Graphics::Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}
