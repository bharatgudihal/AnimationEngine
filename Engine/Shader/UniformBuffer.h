#pragma once
#include <stdint.h>
#include <Engine/Shader/UniformBufferTypes.h>

namespace Engine {
	namespace Graphics {
		class UniformBuffer {
		public:
			static UniformBuffer* CreateUniformBuffer(const UniformBuffers::UniformBufferTypes type, const void* data);
			static void DestroyUniformBuffer(UniformBuffer* uniformBuffer);
			void Update(const void* data);
		private:
			UniformBuffer(const UniformBuffers::UniformBufferTypes type, const void* data);
			~UniformBuffer();
			size_t dataSize;
			unsigned int bufferId;
			UniformBuffers::UniformBufferTypes type;
		};
	}
 }