#pragma once
#include <Engine/Actor/Actor.h>

inline glm::vec3 Engine::Lighting::SimpleLight::GetPosition() const
{
	return actor->transform.position;
}