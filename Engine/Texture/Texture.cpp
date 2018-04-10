#include "Texture.h"

void Engine::Graphics::Texture::DestroyTexture(Texture * texture)
{
	if (!texture->DecrementReferenceCount()) {
		delete texture;
	}
}

const unsigned int Engine::Graphics::Texture::GetTextureId() const
{
	return textureId;
}
