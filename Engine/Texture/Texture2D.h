#pragma once
#include <Engine/Texture/Texture.h>

namespace Engine {
	namespace Graphics {
		class Texture2D : public Texture {
		public:
			static Texture2D* CreateTexture(const char* textureFileName, const bool useGammaCorrection = false);
			static Texture2D* CreateTexture(const unsigned int width, const unsigned int height, const unsigned int internalFormat, const unsigned int pixelFormat);
			static void DestroyTexture(Texture2D* texture);
			void Bind(const unsigned int textureUnit = 0);
			void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam);
			void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam);
			const unsigned int GetTextureId() const;

		private:
			Texture2D(const char* textureFileName, const unsigned int width, const unsigned int height, const unsigned int internalFormat, const unsigned int pixelFormat, const bool useGammaCorrection = false);
			Texture2D();
			Texture2D(Texture2D& other);
			Texture2D& operator=(Texture2D& other);
			~Texture2D();
		};
	}
}