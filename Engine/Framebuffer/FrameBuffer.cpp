#include "Framebuffer.h"
#include <glad/glad.h>
#include <Engine/Texture/Texture.h>
#include <iostream>
#include <cassert>

Engine::Graphics::Framebuffer::Framebuffer(const unsigned int width, const unsigned int height, Texture* texture, bool createStencilBuffer)
{
	//Create frame buffer
	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);	

	//Attach texture to the frame buffer
	if (texture) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureId(), 0);
	}

	//Create render buffer
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	storageType = createStencilBuffer ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT24;
	glRenderbufferStorage(GL_RENDERBUFFER, storageType, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Attach render buffer to frame buffer
	unsigned int attachmentType = createStencilBuffer ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderBufferId);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR:FRAMEBUFFER::Frame buffer is not complete" << std::endl;
	}

	//Unbind frame buffer	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::Graphics::Framebuffer::AttachTexture(const unsigned int textureTarget, const Texture * texture, const unsigned int mipLevel)
{	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, texture->GetTextureId(), mipLevel);
}

void Engine::Graphics::Framebuffer::Resize(const unsigned int newWidth, const unsigned int newHeight)
{
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, storageType, newWidth, newHeight);
}

void Engine::Graphics::Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

void Engine::Graphics::Framebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Engine::Graphics::Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(1, &renderBufferId);
	glDeleteFramebuffers(1, &frameBufferId);
}
