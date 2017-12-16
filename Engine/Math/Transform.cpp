#include "Transform.h"
#include <Engine/Math/Math.h>

Engine::Math::Transform::Transform() : position(), scale(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 0.0f, 1.0f), 
forward(0.0f,0.0f,1.0f), up(0.0f,1.0f,0.0f), rotationAngle(0.0f)
{
}

void Engine::Math::Transform::Rotate(float angle, const glm::vec3& axis)
{
	rotationAngle = angle;
	rotationAxis = axis;
	glm::mat4 rotationMatrix;
	rotationMatrix = glm::rotate(rotationMatrix, rotationAngle, rotationAxis);
	forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 1.0f)));
	up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(up, 1.0f)));
}

void Engine::Math::Transform::RotateDegrees(float angleDegrees, const glm::vec3& axis)
{
	float angleRadians = glm::radians(angleDegrees);
	Rotate(angleRadians, axis);
}
