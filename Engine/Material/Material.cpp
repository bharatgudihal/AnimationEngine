#include "Material.h"
#include <Engine/Texture/Texture.h>
#include <Engine/Shader/Shader.h>

Engine::Graphics::Material::Material(Texture * i_diffuseTexture, Texture * i_specularTexture, const float i_shininess, glm::vec3 i_diffuseColor, glm::vec3 i_specularColor):
	diffuseTexture(i_diffuseTexture), specularTexture(i_specularTexture), diffuseColor(i_diffuseColor), specularColor(i_specularColor),
	shininess(i_shininess), heightScale(0.0f)
{
	if (diffuseTexture) {
		diffuseTexture->IncrementReferenceCount();
	}

	if (specularTexture) {
		specularTexture->IncrementReferenceCount();
	}
}

Engine::Graphics::Material * Engine::Graphics::Material::CreateMaterial(Texture * diffuseTexture, Texture * specularTexture, glm::vec3 diffuseColor, glm::vec3 specularColor, const float shininess)
{
	return new Material(diffuseTexture, specularTexture, shininess, diffuseColor, specularColor);
}

void Engine::Graphics::Material::DestroyMaterial(Material * material)
{
	if (material && !material->DecrementReferenceCount()) {
		delete material;
	}
}

void Engine::Graphics::Material::Bind(Shader* shader)
{
	shader->Use();
	if (diffuseTexture) {
		shader->SetInt("material.diffuse", 0);
		shader->SetBool("material.hasDiffuse", true);
		diffuseTexture->Bind(0);
	}
	else {
		shader->SetBool("material.hasDiffuse", false);
		shader->SetVector("material.diffuseColor", diffuseColor);
	}

	if (specularTexture) {
		shader->SetInt("material.specular", 1);		
		shader->SetBool("material.hasSpecular", true);
		specularTexture->Bind(1);
	}
	else {
		shader->SetBool("material.hasSpecular", false);
		shader->SetVector("material.specularColor", specularColor);
	}

	if (normalMap) {
		shader->SetInt("material.normalMap", 2);
		shader->SetBool("material.hasNormalMap", true);
		normalMap->Bind(2);
	}
	else {
		shader->SetBool("material.hasNormalMap", false);
	}

	if (depthMap) {
		shader->SetInt("material.depthMap", 3);
		shader->SetBool("material.hasDepthMap", true);
		depthMap->Bind(3);
		shader->SetFloat("material.heightScale", heightScale);
	}
	else {
		shader->SetBool("material.hasDepthMap", false);
	}

	shader->SetFloat("material.shininess", shininess);

	//PBR properties
	shader->SetVector("material.albedoColor", albedoColor);
	shader->SetFloat("material.metalness", metalness);
	shader->SetFloat("material.roughness", roughness);
	shader->SetFloat("material.ao", ao);

	if (albedoMap) {
		shader->SetInt("material.albedoMap", 4);
		shader->SetBool("material.hasAlbedoMap", true);
		albedoMap->Bind(4);
	}

	if (metallicMap) {
		shader->SetInt("material.metallicMap", 5);
		shader->SetBool("material.hasMetallicMap", true);
		metallicMap->Bind(5);
	}

	if (roughnessMap) {
		shader->SetInt("material.roughnessMap", 6);
		shader->SetBool("material.hasRoughnessMap", true);
		roughnessMap->Bind(6);
	}

	if (ambientOcclusionMap) {
		shader->SetInt("material.aoMap", 7);
		shader->SetBool("material.hasAoMap", true);
		ambientOcclusionMap->Bind(7);
	}
}

void SetTexture(Engine::Graphics::Texture*& oldTexture, Engine::Graphics::Texture* newTexture) {
	if (oldTexture) {
		Engine::Graphics::Texture::DestroyTexture(oldTexture);
	}
	oldTexture = newTexture;

	if (oldTexture) {
		oldTexture->IncrementReferenceCount();
	}
}

void Engine::Graphics::Material::SetDiffuseTexture(Texture * newTexture)
{
	SetTexture(diffuseTexture, newTexture);
}

void Engine::Graphics::Material::SetSpecularTexture(Texture * newTexture)
{
	SetTexture(specularTexture, newTexture);
}

void Engine::Graphics::Material::SetNormalMap(Texture * newTexture)
{
	SetTexture(normalMap, newTexture);
}

void Engine::Graphics::Material::SetDepthMap(Texture * newTexture, const float newHeightScale)
{
	SetTexture(depthMap, newTexture);
	heightScale = newHeightScale;
}

void Engine::Graphics::Material::SetAlbedoMap(Texture * newTexture)
{
	SetTexture(albedoMap, newTexture);
}

void Engine::Graphics::Material::SetMetallicMap(Texture * newTexture)
{
	SetTexture(metallicMap, newTexture);
}

void Engine::Graphics::Material::SetRoughnessMap(Texture * newTexture)
{
	SetTexture(roughnessMap, newTexture);
}

void Engine::Graphics::Material::SetAmbientOcclusionMap(Texture * newTexture)
{
	SetTexture(ambientOcclusionMap, newTexture);
}

Engine::Graphics::Material::~Material()
{
	if (diffuseTexture) {
		Texture::DestroyTexture(diffuseTexture);
	}

	if (specularTexture) {
		Texture::DestroyTexture(specularTexture);
	}

	if (normalMap) {
		Texture::DestroyTexture(normalMap);
	}

	if (depthMap) {
		Texture::DestroyTexture(depthMap);
	}

	if (albedoMap) {
		Texture::DestroyTexture(albedoMap);
	}

	if (metallicMap) {
		Texture::DestroyTexture(metallicMap);
	}

	if (roughnessMap) {
		Texture::DestroyTexture(roughnessMap);
	}

	if (ambientOcclusionMap) {
		Texture::DestroyTexture(ambientOcclusionMap);
	}
}
