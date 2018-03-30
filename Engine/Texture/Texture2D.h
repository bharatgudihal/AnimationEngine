#pragma once

#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture2D {
		public:
			static Texture2D* CreateTexture(const char* textureFileName);
			static Texture2D* CreateTexture(const unsigned int width, const unsigned int height, const unsigned int pixelFormat);
			static void DestroyTexture(Texture2D* texture);
			void Bind(const unsigned int textureUnit = 0);
			void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int maxFilterParam);
			void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam);
			const unsigned int GetTextureId() const;
			REFERENCE_COUNT_FUNCTIONS

		private:
			Texture2D(const char* textureFileName, const unsigned int width, const unsigned int height, const unsigned int pixelFormat);
			Texture2D();
			Texture2D(Texture2D& other);
			Texture2D& operator=(Texture2D& other);
			~Texture2D();
			unsigned int textureId;
			REFERENCE_COUNT_VARIABLES
		};
	}
}