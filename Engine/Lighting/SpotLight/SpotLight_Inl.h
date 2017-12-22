#include "SpotLight.h"
#pragma once

inline void Engine::Lighting::SpotLight::SetInnerCutOffDegrees(float i_cutOffDegrees)
{
	innerCutOff = glm::radians(i_cutOffDegrees);
}

inline void Engine::Lighting::SpotLight::SetInnerCutOff(float i_cutOff)
{
	innerCutOff = i_cutOff;
}

inline float Engine::Lighting::SpotLight::GetInnerCutOff() const
{
	return innerCutOff;
}

inline void Engine::Lighting::SpotLight::SetOuterCutOffDegrees(float cutOffDegrees)
{
	outerCutOff = glm::radians(cutOffDegrees);
}

inline void Engine::Lighting::SpotLight::SetOuterCutOff(float cutOff)
{
	outerCutOff = cutOff;
}

inline float Engine::Lighting::SpotLight::GetOuterCutOff() const
{
	return outerCutOff;
}

inline void Engine::Lighting::SpotLight::SetDirection(glm::vec3 i_direction)
{
	direction = i_direction;
}

inline glm::vec3 Engine::Lighting::SpotLight::GetDirection() const
{
	return direction;
}
