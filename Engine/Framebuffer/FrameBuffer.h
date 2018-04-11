#pragma once

namespace Engine {
	namespace Graphics {
		
		class Texture;
		class Framebuffer {
		public:
			Framebuffer(const unsigned int width, const unsigned int height, Texture* texture, bool createStencilBuffer = false);
			void AttachTexture(const unsigned int textureTarget, const Texture* texture, const unsigned int mipLevel = 0);
			void Resize(const unsigned int newWidth, const unsigned int newHeight);
			void Bind();
			void UnBind();
			~Framebuffer();
		private:
			Framebuffer();
			Framebuffer(Framebuffer& other);
			Framebuffer& operator=(Framebuffer& other);
			unsigned int frameBufferId;
			unsigned int renderBufferId;
			unsigned int storageType;
		};
	}
}