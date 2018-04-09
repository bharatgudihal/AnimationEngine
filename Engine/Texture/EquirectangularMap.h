#pragma once
#include "Texture.h"
#include <string>

namespace Engine {
	namespace Graphics {
		class EquirectangularMap : public Texture {
			static EquirectangularMap* CreateEquirectangularMap(const std::string& textureFile);
			static void DestroyEquirectangularMap(EquirectangularMap* equirectangularMap);
			virtual void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam);
			virtual void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam);
			virtual void Bind(unsigned int textureUnit = 0);
		private:
			EquirectangularMap();
			EquirectangularMap(const std::string& textureFile);
			~EquirectangularMap();
			EquirectangularMap(EquirectangularMap& other);
			EquirectangularMap& operator=(EquirectangularMap& other);
		};
	}
}