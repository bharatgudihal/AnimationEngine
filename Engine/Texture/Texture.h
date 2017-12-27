#pragma once

#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			static Texture* CreateTexture(const char* textureFileName, const unsigned int textureUnit = 0);
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

			unsigned int textureId;
			unsigned int textureUnit : 4;
			REFERENCE_COUNT_VARIABLES
		};
	}
}