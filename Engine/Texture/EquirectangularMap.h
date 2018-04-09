#pragma once
#include "Texture.h"
#include <string>

namespace Engine {
	namespace Graphics {
		class EquirectangularMap : public Texture{
			static EquirectangularMap* CreateEquirectangularMap(const std::string& textureFile);
			static void DestroyEquirectangularMap(EquirectangularMap* cubeMap);

		private:
			EquirectangularMap();
			EquirectangularMap(const std::string& textureFile);
			~EquirectangularMap();
			EquirectangularMap(EquirectangularMap& other);
			EquirectangularMap& operator=(EquirectangularMap& other);
		};
	}
}