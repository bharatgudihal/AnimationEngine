#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			struct DataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
				glm::vec4 viewPos;
				glm::vec4 lightVector;
				glm::vec4 lightAmbient;
				glm::vec4 lightDiffuse;
				glm::vec4 lightSpecular;
			};
		}
	}
}