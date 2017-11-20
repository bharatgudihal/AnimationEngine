#include "Transform.h"

Engine::Graphics::Math::Transform::Transform() :scale(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 0.0f, 1.0f), rotationAngle(0.0f)
{
}

void Engine::Graphics::Math::Transform::Rotate(float angle, Vector3 axis)
{
	rotationAngle = angle;
	rotationAxis = axis;
}
