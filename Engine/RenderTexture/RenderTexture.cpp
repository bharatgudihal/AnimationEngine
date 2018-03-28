#include "RenderTexture.h"
#include <glad/glad.h>
#include <Engine/Texture/Texture.h>
#include <iostream>

Engine::Graphics::RenderTexture::RenderTexture(const unsigned int width, const unsigned int height, const unsigned int pixelFormat)
{
	//Create frame buffer
	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	//Create texture
	texture = Engine::Graphics::Texture::CreateTexture(width, height, pixelFormat);
	texture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);

	//Attach texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureId(), 0);

	//Create render buffer
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Attach render buffer to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR:FRAMEBUFFER::Frame buffer is not complete" << std::endl;
	}

	//Unbind frame buffer	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::Graphics::RenderTexture::SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int maxFilterParam)
{
	texture->SetTextureFilteringParams(minFilterParam, maxFilterParam);
}

void Engine::Graphics::RenderTexture::SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam)
{
	texture->SetTextureWrappingParams(sWrappingParam, tWrappingParam);
}

void Engine::Graphics::RenderTexture::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void Engine::Graphics::RenderTexture::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Engine::Graphics::Texture* Engine::Graphics::RenderTexture::GetTexture() {
	return texture;
}

Engine::Graphics::RenderTexture::~RenderTexture()
{
	glDeleteRenderbuffers(1, &renderBufferId);
	Engine::Graphics::Texture::DestroyTexture(texture);
	glDeleteFramebuffers(1, &frameBufferId);
}
