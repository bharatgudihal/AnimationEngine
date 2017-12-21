#pragma once
#include <Engine/Actor/Actor.h>

inline glm::vec3 Engine::Lighting::PointLight::GetPosition() const
{
	return actor->transform.position;
}