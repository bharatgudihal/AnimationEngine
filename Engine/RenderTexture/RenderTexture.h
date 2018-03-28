#pragma once

namespace Engine {
	namespace Graphics {
		
		class Texture;
		class RenderTexture {
		public:
			RenderTexture(const unsigned int width, const unsigned int height, const unsigned int pixelFormat);
			void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int maxFilterParam);
			void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam);
			void Bind();
			void UnBind();
			Texture* GetTexture();
			~RenderTexture();
		private:
			RenderTexture();
			RenderTexture(RenderTexture& other);
			void operator=(RenderTexture& other);
			Texture* texture;
			unsigned int frameBufferId;
			unsigned int renderBufferId;
		};
	}
}