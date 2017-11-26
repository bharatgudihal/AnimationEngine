#pragma once
#include <Externals/glm/Includes.h> 
#include <Engine/Math/Transform.h>

namespace Engine {
	namespace Graphics {
		class Camera {
		public:
			Camera(const float FOV, const float aspectRatio, const float nearPlane, const float farPlane);
			~Camera();
			
			glm::mat4 GetViewMatrix() const;
			glm::mat4 GetProjectionMatrix() const;
			
			Math::Transform transform;
			
			float FOV;
			float aspectRatio;
			float nearPlane;
			float farPlane;			
		};
	}
}