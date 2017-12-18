#pragma once
#include <stdint.h>

namespace Engine {
	namespace Graphics {

		enum UniformBufferType : uint8_t {
			DataPerFrame = 0
		};
		
		class UniformBuffer {
		public:
			UniformBuffer(const UniformBufferType type, const unsigned int usage);
			~UniformBuffer();
			void Update(const void* data);
		private:
			unsigned int bufferId;
			size_t dataSize;
		};
	}
}