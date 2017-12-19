#pragma once

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			static Texture* CreateTexture(const char* textureFileName, const unsigned int textureUnit = 0);
			static void DestroyTexture(Texture* texture);
			void Bind();
			const unsigned int GetTextureUnit() const;

		private:
			Texture(const char* textureFileName, const unsigned int textureUnit);
			Texture();
			~Texture();

			unsigned int textureId;
			unsigned int textureUnit : 4;
		};
	}
}