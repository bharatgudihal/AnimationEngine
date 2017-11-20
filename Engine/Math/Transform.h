#pragma once
#include "Vector3.h"

namespace Engine {
	namespace Graphics {
		namespace Math {
			class Transform {

			public:
				Transform();
				void Rotate(float angleRadians, Vector3 axis);

				Vector3 position;
				Vector3 scale;
				Vector3 rotationAxis;
				float rotationAngle;
			};
		}
	}
}