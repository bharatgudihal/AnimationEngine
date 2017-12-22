#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			struct LightData {
				glm::vec4 vector;
				
				glm::vec4 ambient;
				glm::vec4 diffuse;
				glm::vec4 specular;
								
				float linear;
				float quadratic;				
			};

			struct DataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
				glm::vec4 viewPos;
				
				LightData lightData;
				
				float padding[2];
			};
		}
	}
}