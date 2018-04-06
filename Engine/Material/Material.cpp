#include "Material.h"
#include <Engine/Texture/Texture.h>
#include <Engine/Shader/Shader.h>

Engine::Graphics::Material::Material(Texture * i_diffuseTexture, Texture * i_specularTexture, const float i_shininess, glm::vec3 i_diffuseColor, glm::vec3 i_specularColor):
	diffuseTexture(i_diffuseTexture), specularTexture(i_specularTexture), diffuseColor(i_diffuseColor), specularColor(i_specularColor), shininess(i_shininess)
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
	}
	else {
		shader->SetBool("material.hasDepthMap", false);
	}

	shader->SetFloat("material.shininess", shininess);
}

void Engine::Graphics::Material::SetDiffuseTexture(Texture * newTexture)
{
	if (diffuseTexture) {
		Texture::DestroyTexture(diffuseTexture);
	}
	diffuseTexture = newTexture;
	
	if (diffuseTexture) {
		diffuseTexture->IncrementReferenceCount();
	}
}

void Engine::Graphics::Material::SetSpecularTexture(Texture * newTexture)
{
	if (specularTexture) {
		Texture::DestroyTexture(specularTexture);
	}
	specularTexture = newTexture;
	if (specularTexture) {
		specularTexture->IncrementReferenceCount();
	}
}

void Engine::Graphics::Material::SetDiffuseColor(const glm::vec3 newColor)
{
	diffuseColor = newColor;
}

void Engine::Graphics::Material::SetSpecularColor(const glm::vec3 newColor)
{
	specularColor = newColor;
}

void Engine::Graphics::Material::SetNormalMap(Texture * newTexture)
{
	if (normalMap) {
		Texture::DestroyTexture(normalMap);
	}
	normalMap = newTexture;
	if (normalMap) {
		normalMap->IncrementReferenceCount();
	}
}

void Engine::Graphics::Material::SetDepthMap(Texture * newTexture)
{
	if (depthMap) {
		Texture::DestroyTexture(depthMap);
	}
	depthMap = newTexture;

	if (depthMap) {
		depthMap->IncrementReferenceCount();
	}
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
}
