#include "Camera.h"
#include <Engine/Math/Math.h>
#include <GLFW\glfw3.h>

Engine::Graphics::Camera::Camera(const float FOVDegrees, const float aspectRatio, const float nearPlane, const float farPlane):
	FOV(glm::radians(FOVDegrees)),aspectRatio(aspectRatio),nearPlane(nearPlane),farPlane(farPlane)
{
	transform.position.z = 3.0f;
	transform.forward = glm::vec3(0.0f, 0.0f, -1.0f);
}

Engine::Graphics::Camera::~Camera()
{
}

glm::mat4 Engine::Graphics::Camera::GetViewMatrix() const
{	
	glm::vec3 cameraPos = transform.position;
	glm::vec3 cameraFront = transform.forward;
	glm::vec3 cameraUp = transform.up;
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Engine::Graphics::Camera::GetProjectionMatrix() const
{
	return glm::perspective(FOV,aspectRatio,nearPlane,farPlane);
}
