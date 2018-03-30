#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <Externals/STB_Image/stb_image.h>

unsigned char * Engine::Utility::LoadTexture(const char * fileName, int & width, int & height, int & channels, bool flipVertically)
{
	stbi_set_flip_vertically_on_load(flipVertically);
	return stbi_load(fileName, &width, &height, &channels, 0);
}

void Engine::Utility::FreeTexture(unsigned char * data)
{
	stbi_image_free(data);
}
