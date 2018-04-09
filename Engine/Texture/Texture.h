#pragma once
#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			virtual void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam) = 0;
			virtual void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam) = 0;
			virtual void Bind(unsigned int textureUnit = 0) = 0;
			static void DestroyTexture(Texture* texture);
			REFERENCE_COUNT_FUNCTIONS
		protected:
			Texture() {};
			virtual ~Texture() {};
			unsigned int textureId;
		private:			
			REFERENCE_COUNT_VARIABLES
		};
	}
}