#include "Texture.h"

void Engine::Graphics::Texture::DestroyTexture(Texture * texture)
{
	if (!texture->DecrementReferenceCount()) {
		delete texture;
	}
}
