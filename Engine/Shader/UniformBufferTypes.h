#pragma once

#include <stdint.h>
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			
			enum class UniformBufferTypes :uint8_t {
				DATA_PER_FRAME = 0
			};
			
			struct dataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
			};
		}
	}
}