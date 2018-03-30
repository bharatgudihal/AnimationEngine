#include "Texture2D.h"
#include <glad/glad.h>
#include <iostream>
#include <Externals/stb_image/Includes.h>

Engine::Graphics::Texture2D::Texture2D(const char * textureFileName, const unsigned int width, const unsigned int height, const unsigned int pixelFormat) {

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);	
	
	if (textureFileName) {
		//Load texture
		int texureWidth, textureHeight, textureChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(textureFileName, &texureWidth, &textureHeight, &textureChannels, 0);
		if (data) {
			unsigned int texurePixelFormat = GL_RGB;
			switch (textureChannels) {
			case 2:
				texurePixelFormat = GL_RG;
				break;
			case 3:
				texurePixelFormat = GL_RGB;
				break;
			case 4:
				texurePixelFormat = GL_RGBA;
				break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texureWidth, textureHeight, 0, texurePixelFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture in path" << textureFileName;
		}
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, NULL);
	}	
}

Engine::Graphics::Texture2D * Engine::Graphics::Texture2D::CreateTexture(const char * textureFileName)
{
	return new Texture2D(textureFileName,0,0,0);
}

Engine::Graphics::Texture2D * Engine::Graphics::Texture2D::CreateTexture(const unsigned int width, const unsigned int height, const unsigned int pixelFormat)
{
	return new Texture2D(nullptr, width, height, pixelFormat);
}

void Engine::Graphics::Texture2D::DestroyTexture(Texture2D * texture)
{
	if (!texture->DecrementReferenceCount()) {
		delete texture;
	}
}

void Engine::Graphics::Texture2D::Bind(const unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Engine::Graphics::Texture2D::SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int maxFilterParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilterParam);
}

void Engine::Graphics::Texture2D::SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrappingParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrappingParam);
}

const unsigned int Engine::Graphics::Texture2D::GetTextureId() const
{
	return textureId;
}

Engine::Graphics::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &textureId);
}
