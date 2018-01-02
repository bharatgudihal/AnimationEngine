#pragma once

#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			static Texture* CreateTexture(const char* textureFileName);
			static void DestroyTexture(Texture* texture);
			void Bind();
			const unsigned int GetTextureUnit() const;
			REFERENCE_COUNT_FUNCTIONS

		private:
			Texture(const char* textureFileName, const unsigned int textureUnit);
			Texture();
			Texture(Texture& other);
			void operator=(Texture& other);
			~Texture();

			static uint32_t GLOBAL_TEXTURE_COUNT;
			unsigned int textureId;
			uint32_t textureUnit;
			REFERENCE_COUNT_VARIABLES
		};
	}
}