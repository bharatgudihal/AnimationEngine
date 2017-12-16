#pragma once
#include "Transform.h"
#include <Externals/glm/Includes.h>

namespace Engine {
	namespace Math {
		glm::mat4 CalculateTransform(const Transform& transform);
	}
}