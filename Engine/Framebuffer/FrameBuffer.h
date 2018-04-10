#pragma once

namespace Engine {
	namespace Graphics {
		
		class Texture;
		class Framebuffer {
		public:
			Framebuffer(const unsigned int width, const unsigned int height, Texture* texture, bool createStencilBuffer = false);
			void AttachTexture(const unsigned int textureTarget, const Texture* texture);
			void Bind();
			void UnBind();
			~Framebuffer();
		private:
			Framebuffer();
			Framebuffer(Framebuffer& other);
			Framebuffer& operator=(Framebuffer& other);
			unsigned int frameBufferId;
			unsigned int renderBufferId;
		};
	}
}