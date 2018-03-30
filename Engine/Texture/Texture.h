#pragma once
#include <Engine/Utility/ReferenceCounted.h>

namespace Engine {
	namespace Graphics {
		class Texture {
		public:
			virtual void SetTextureFilteringParams(const unsigned int minFilterParam, const unsigned int magFilterParam) = 0;
			virtual void SetTextureWrappingParams(const unsigned int sWrappingParam, const unsigned int tWrappingParam, const unsigned int rWrappingParam) = 0;
			virtual void Bind(unsigned int texterUnit = 0) = 0;
			REFERENCE_COUNT_FUNCTIONS
		protected:
			Texture() {};
			virtual ~Texture() {};
		private:			
			REFERENCE_COUNT_VARIABLES
		};
	}
}