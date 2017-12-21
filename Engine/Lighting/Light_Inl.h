#pragma once
#include <Engine/Actor/Actor.h>

inline void Engine::Lighting::Light::ShowMesh(const bool i_showMesh) {
	showMesh = i_showMesh;
}

inline glm::vec3 Engine::Lighting::Light::GetPosition() const
{
	return actor->transform.position;
}