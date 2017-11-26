#include "Camera.h"
#include <Engine/Math/Math.h>

Engine::Graphics::Camera::Camera(const float FOVDegrees, const float aspectRatio, const float nearPlane, const float farPlane):
	aspectRatio(aspectRatio),nearPlane(nearPlane),farPlane(farPlane)
{
	FOV = Math::CovertDegreesToRadians(FOVDegrees);
}

Engine::Graphics::Camera::~Camera()
{
}

glm::mat4 Engine::Graphics::Camera::GetViewMatrix() const
{
	return Math::CalculateTransform(transform);
}

glm::mat4 Engine::Graphics::Camera::GetProjectionMatrix() const
{
	return glm::perspective(FOV,aspectRatio,nearPlane,farPlane);
}
