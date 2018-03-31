#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Graphics {
		namespace UniformBuffers {
			
			struct LightData {
				glm::vec4 ambient;
				glm::vec4 diffuse;
				glm::vec4 specular;
			};

			struct DirectionalLight {
				glm::vec4 direction;
				
				LightData lightData;
				
				glm::vec4 isActive;
			};

			struct PointLight {				
				glm::vec4 position;
				
				LightData lightData;

				float linear = 0.0f;
				float quadratic = 0.0f;

				float isActive = 0.0f;

				float padding = 0.0f;
			};

			struct SpotLight {
				glm::vec4 direction;
				glm::vec4 position;

				LightData lightData;
				
				float innerCutOff = 1.0f;
				float outerCutOff = 1.0f;
				
				float isActive = 0.0f;

				float padding = 0.0f;
			};

			#define NR_POINT_LIGHTS 4
			
			struct DataPerFrame {
				glm::mat4 view;
				glm::mat4 projection;
				glm::vec4 viewPos;
				
				DirectionalLight directionalLight;

				PointLight pointLights[NR_POINT_LIGHTS];

				SpotLight spotLight;
			};
		}
	}
}