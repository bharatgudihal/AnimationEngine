#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			struct LightData {
				glm::vec4 vector;
				glm::vec4 direction;
				
				glm::vec4 ambient;
				glm::vec4 diffuse;
				glm::vec4 specular;
								
				float linear = 0.0f;
				float quadratic = 0.0f;

				float isSpotLight = 0.0;
				float innerCutOff = 1.0f;
				float outerCutOff = 1.0f;

				float padding[3];
			};

			struct DataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
				glm::vec4 viewPos;
				
				LightData lightData;				
			};
		}
	}
}