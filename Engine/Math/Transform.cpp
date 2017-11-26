#include "Transform.h"
#include <Engine/Math/Math.h>

Engine::Graphics::Math::Transform::Transform() :scale(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 0.0f, 1.0f), rotationAngle(0.0f)
{
}

void Engine::Graphics::Math::Transform::Rotate(float angle, Vector3 axis)
{
	rotationAngle = angle;
	rotationAxis = axis;
}

void Engine::Graphics::Math::Transform::RotateDegrees(float angleDegrees, Vector3 axis)
{
	float angleRadians = Math::CovertDegreesToRadians(angleDegrees);
	Rotate(angleRadians, axis);
}
