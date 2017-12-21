#pragma once

inline glm::vec3 Engine::Lighting::DirectionalLight::GetLightDirection() const {
	return lightDirection;
}