#pragma once
#include <Engine/Actor/Actor.h>

inline void Engine::Lighting::Light::ShowMesh(const bool i_showMesh) {
	showMesh = i_showMesh;
}

inline void Engine::Lighting::Light::SetPosition(glm::vec3 i_position)
{
	actor->transform.position = i_position;
}

inline glm::vec3 Engine::Lighting::Light::GetPosition() const
{
	return actor->transform.position;
}