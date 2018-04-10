#include "CubeMap.h"
#include <glad/glad.h>
#include <Engine/Utility/TextureLoader.h>
#include <cassert>
#include <iostream>

Engine::Graphics::CubeMap * Engine::Graphics::CubeMap::CreateCubeMap(const std::vector<std::string>& textureFiles)
{
	return new CubeMap(textureFiles, 0, 0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}

Engine::Graphics::CubeMap* Engine::Graphics::CubeMap::CreateCubeMap(const unsigned int width, const unsigned int height, 
	const unsigned int internalFormat, const unsigned int pixelFormat, const unsigned int dataType)
{
	std::vector<std::string> textureFiles;
	return new CubeMap(textureFiles, width, height, internalFormat, pixelFormat, dataType);
}

void Engine::Graphics::CubeMap::DestroyCubeMap(CubeMap * cubeMap)
{
	if (!cubeMap->DecrementReferenceCount()) {
		delete cubeMap;
	}
}

void Engine::Graphics::CubeMap::Bind(const unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

void Engine::Graphics::CubeMap::SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilterParam);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilterParam);
}

void Engine::Graphics::CubeMap::SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, sWrappingParam);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tWrappingParam);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, rWrappingParam);
}

void Engine::Graphics::CubeMap::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Engine::Graphics::CubeMap::CubeMap(const std::vector<std::string>& textureFiles, const unsigned int width, const unsigned int height,
	const unsigned int internalFormat, const unsigned int pixelFormat, const unsigned int dataType)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	assert(textureFiles.size() == 6 || (textureFiles.size() == 0 && width != 0 && height != 0));

	int texWidth, texHeight, texChannels;
	unsigned int texPixelFormat;
	unsigned char* data;
	bool loadingSuccess = true;
	for (int i = 0; i < 6; i++) {
		if (textureFiles.size() != 0) {
			data = Engine::Utility::LoadTexture(textureFiles[i].c_str(), texWidth, texHeight, texChannels, 0);
			if (data) {
				switch (texChannels) {
				case 2:
					texPixelFormat = GL_RG;
					break;
				case 3:
					texPixelFormat = GL_RGB;
					break;
				case 4:
					texPixelFormat = GL_RGBA;
					break;
				}
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texWidth, texHeight, 0, texPixelFormat, GL_UNSIGNED_BYTE, data);
				Engine::Utility::FreeTexture(data);
			}
			else {
				std::cout << "Failed to load image from path " << textureFiles[i] << std::endl;
				loadingSuccess = false;
				break;
			}
		}
		else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, pixelFormat, dataType, nullptr);
		}
	}

	if (loadingSuccess) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

Engine::Graphics::CubeMap::~CubeMap()
{
	glDeleteTextures(1, &textureId);
}
