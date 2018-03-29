#pragma once

#include <Engine/Utility/ReferenceCounted.h>
#include <vector>
#include <string>

namespace Engine {
	namespace Graphics {
		class CubeMap {
		public:
			static CubeMap* CreateCubeMap(const std::vector<std::string>& textureFiles);
			static void DestroyCubeMap(CubeMap* cubeMap);
			void Bind(const unsigned int textureUnit = 0);
			void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int maxFilterParam);
			void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam);
			REFERENCE_COUNT_FUNCTIONS
		private:
			CubeMap();
			CubeMap(const std::vector<std::string>& textureFiles);
			~CubeMap();
			CubeMap(CubeMap& other);
			CubeMap& operator=(CubeMap& other);
			unsigned int textureId;
			REFERENCE_COUNT_VARIABLES
		};
	}
}