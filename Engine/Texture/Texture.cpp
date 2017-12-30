#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <Externals/stb_image/Includes.h>

unsigned int Engine::Graphics::Texture::GLOBAL_TEXTURE_COUNT = 0;

Engine::Graphics::Texture::Texture(const char * textureFileName, const unsigned int textureUnit) {

	this->textureUnit = textureUnit;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	// Set texture wrapping params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName, &width, &height, &nrChannels, 0);
	if (data) {
		unsigned int pixelFormat = GL_RGB;
		if (nrChannels == 4) {
			pixelFormat = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture in path" << textureFileName;
	}
	stbi_image_free(data);
}

Engine::Graphics::Texture * Engine::Graphics::Texture::CreateTexture(const char * textureFileName)
{
	return new Texture(textureFileName, GLOBAL_TEXTURE_COUNT++);
}

void Engine::Graphics::Texture::DestroyTexture(Texture * texture)
{
	if (!texture->DecrementReferenceCount()) {
		delete texture;
	}
}

void Engine::Graphics::Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

const unsigned int Engine::Graphics::Texture::GetTextureUnit() const {
	return textureUnit;
}

Engine::Graphics::Texture::~Texture()
{
}
