#pragma once

namespace Engine {
	namespace Utility {
		unsigned char* LoadTexture(const char* fileName, int& width, int& height, int& channels, bool flipVertically = false);
		void FreeTexture(unsigned char* data);
	}
}