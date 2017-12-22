#include "SpotLight.h"
#pragma once

inline void Engine::Lighting::SpotLight::SetCutOffDegrees(float i_cutOffDegrees)
{
	cutOff = glm::radians(i_cutOffDegrees);
}

inline void Engine::Lighting::SpotLight::SetCutOff(float i_cutOff)
{
	cutOff = i_cutOff;
}

inline float Engine::Lighting::SpotLight::GetCutOff() const
{
	return cutOff;
}

inline void Engine::Lighting::SpotLight::SetDirection(glm::vec3 i_direction)
{
	direction = i_direction;
}

inline glm::vec3 Engine::Lighting::SpotLight::GetDirection() const
{
	return direction;
}
