#pragma once
#include <Engine/Texture/Texture.h>
#include <vector>
#include <string>

namespace Engine {
	namespace Graphics {
		class CubeMap : public Texture {
		public:
			//Textures should be in the order: right,left,top,bottom,front,back
			static CubeMap* CreateCubeMap(const std::vector<std::string>& textureFiles);
			static void DestroyCubeMap(CubeMap* cubeMap);
			void Bind(const unsigned int textureUnit = 0);
			void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam);
			void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam);

		private:
			CubeMap();
			CubeMap(const std::vector<std::string>& textureFiles);
			~CubeMap();
			CubeMap(CubeMap& other);
			CubeMap& operator=(CubeMap& other);
		};
	}
}