#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			struct DataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
				glm::vec3 viewPos;
				const float a = 0;
				glm::vec3 lightPosition;
				const float b = 0;
				glm::vec3 lightAmbient;
				const float c = 0;
				glm::vec3 lightDiffuse;
				const float d = 0;
				glm::vec3 lightSpecular;
				const float e = 0;
			};
		}
	}
}