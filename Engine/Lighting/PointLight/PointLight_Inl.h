#include "PointLight.h"
#pragma once

inline void Engine::Lighting::PointLight::SetAttenuation(Attenuation & i_attenuation)
{
	attenuation = i_attenuation;
}

inline Engine::Lighting::Attenuation Engine::Lighting::PointLight::GetAttenuation() const
{
	return attenuation;
}
