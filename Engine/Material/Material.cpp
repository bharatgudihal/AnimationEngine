#include "Material.h"
#include <Engine/Texture/Texture.h>
#include <Engine/Shader/Shader.h>

Engine::Graphics::Material::Material(Texture2D * i_diffuseTexture, Texture2D * i_specularTexture, const float i_shininess, glm::vec3 i_diffuseColor, glm::vec3 i_specularColor):
	diffuseTexture(i_diffuseTexture), specularTexture(i_specularTexture), diffuseColor(i_diffuseColor), specularColor(i_specularColor), shininess(i_shininess)
{
	if (diffuseTexture) {
		diffuseTexture->IncrementReferenceCount();
	}

	if (specularTexture) {
		specularTexture->IncrementReferenceCount();
	}
}

Engine::Graphics::Material * Engine::Graphics::Material::CreateMaterial(Texture2D * diffuseTexture, Texture2D * specularTexture, glm::vec3 diffuseColor, glm::vec3 specularColor, const float shininess)
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

	shader->SetFloat("material.shininess", shininess);
}

Engine::Graphics::Material::~Material()
{
	if (diffuseTexture && !diffuseTexture->DecrementReferenceCount()) {
		Texture2D::DestroyTexture(diffuseTexture);
	}

	if (specularTexture && !specularTexture->DecrementReferenceCount()) {
		Texture2D::DestroyTexture(specularTexture);
	}
}
