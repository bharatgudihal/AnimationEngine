#pragma once

inline void Engine::Graphics::Material::SetAlbedoColor(const glm::vec3 albedoColor) {
	this->albedoColor = albedoColor;
}

inline void Engine::Graphics::Material::SetMetalness(const float metalness)
{
	this->metalness = metalness;
}

inline void Engine::Graphics::Material::SetRoughness(const float roughness)
{
	this->roughness = roughness;
}

inline void Engine::Graphics::Material::SetAmbientOcclusion(const float ao)
{
	this->ao = ao;
}

inline void Engine::Graphics::Material::SetDiffuseColor(const glm::vec3 newColor)
{
	diffuseColor = newColor;
}

inline void Engine::Graphics::Material::SetSpecularColor(const glm::vec3 newColor)
{
	specularColor = newColor;
}