#pragma once

namespace Engine {
	namespace Graphics {
		namespace Math {
			class Vector3 {
			public:
				Vector3();
				Vector3(float x, float y, float z);				
				float x, y, z;

				static Vector3 UP;
				static Vector3 ZERO;
				static Vector3 RIGHT;
				static Vector3 FORWARD;
			};			
		}
	}
}