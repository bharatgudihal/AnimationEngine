#include "Texture2D.h"
#include <glad/glad.h>
#include <iostream>
#include <Engine/Utility/TextureLoader.h>

Engine::Graphics::Texture2D::Texture2D(const char * textureFileName, const unsigned int width, const unsigned int height,
	const unsigned int internalFormat, const unsigned int pixelFormat, const unsigned int dataType, const bool useGammaCorrection, const bool isHDR) {

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);	
	
	if (textureFileName) {
		//Load texture
		int textureWidth, textureHeight, textureChannels;
		void* data;
		if (isHDR) {
			data = Utility::LoadHDRTexture(textureFileName, textureWidth, textureHeight, textureChannels, true);
		}
		else {
			data = Engine::Utility::LoadTexture(textureFileName, textureWidth, textureHeight, textureChannels, false);
		}
		if (data) {
			unsigned int texturePixelFormat = GL_RGB;
			unsigned int internalTextureFormat = isHDR ? GL_RGB16F : useGammaCorrection ? GL_SRGB : GL_RGB;
			switch (textureChannels) {
			case 1:
				texturePixelFormat = GL_RED;
				internalTextureFormat = GL_RED;
				break;
			case 2:
				texturePixelFormat = GL_RG;
				internalTextureFormat = isHDR ? GL_RG16F : GL_RG;
				break;
			case 3:
				texturePixelFormat = GL_RGB;
				internalTextureFormat = isHDR ? GL_RGB16F : useGammaCorrection ? GL_SRGB : GL_RGB;
				break;
			case 4:
				texturePixelFormat = GL_RGBA;
				internalTextureFormat = isHDR ? GL_RGBA16F : useGammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
				break;
			}

			unsigned int dataType = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTexImage2D(GL_TEXTURE_2D, 0, internalTextureFormat, textureWidth, textureHeight, 0, texturePixelFormat, dataType, data);

			Engine::Utility::FreeTexture(data);
		}
		else {
			std::cout << "Failed to load texture in path " << textureFileName << std::endl;
		}
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, pixelFormat, dataType, NULL);
	}	
}

Engine::Graphics::Texture2D * Engine::Graphics::Texture2D::CreateTexture(const char * textureFileName, const bool useGammaCorrection)
{
	return new Texture2D(textureFileName, 0, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, useGammaCorrection, false);
}

Engine::Graphics::Texture2D * Engine::Graphics::Texture2D::CreateHDRTexture(const char * textureFileName, const bool useGammaCorrection)
{
	return new Texture2D(textureFileName, 0, 0, GL_RGB, GL_RGB, GL_FLOAT, useGammaCorrection, true);
}

Engine::Graphics::Texture2D * Engine::Graphics::Texture2D::CreateTexture(const unsigned int width, const unsigned int height,
	const unsigned int internalFormat, const unsigned int pixelFormat, const unsigned int dataType)
{
	return new Texture2D(nullptr, width, height, internalFormat, pixelFormat, dataType, false, false);
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

void Engine::Graphics::Texture2D::SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterParam);
}

void Engine::Graphics::Texture2D::SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrappingParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrappingParam);
}

void Engine::Graphics::Texture2D::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Engine::Graphics::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &textureId);
}
