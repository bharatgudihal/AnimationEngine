#pragma once

#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			static Texture* CreateTexture(const char* textureFileName);
			static Texture* CreateTexture(unsigned int width, unsigned int height, const unsigned int pixelFormat);
			static void DestroyTexture(Texture* texture);
			void Bind(const unsigned int textureUnit = 0);
			void SetTextureFilteringParams(unsigned int minFilterParam, unsigned int maxFilterParam);
			void SetTextureWrappingParams(unsigned int sWrappingParam, unsigned int tWrappingParam);
			REFERENCE_COUNT_FUNCTIONS

		private:
			Texture(const char* textureFileName, const unsigned int width, const unsigned int height, const unsigned int pixelFormat);
			Texture();
			Texture(Texture& other);
			void operator=(Texture& other);
			~Texture();

			static uint32_t GLOBAL_TEXTURE_COUNT;
			unsigned int textureId;
			REFERENCE_COUNT_VARIABLES
		};
	}
}