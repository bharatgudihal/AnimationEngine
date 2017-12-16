#pragma once
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Math {
		class Transform {

		public:
			Transform();
			void Rotate(float angleRadians, const glm::vec3& axis);
			void RotateDegrees(float angleDegrees, const glm::vec3& axis);

			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 rotationAxis;
			glm::vec3 forward;
			glm::vec3 up;
			float rotationAngle;
		};
	}
}