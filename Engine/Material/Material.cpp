#include "Material.h"
#include <Engine/Texture/Texture.h>
#include <Engine/Shader/Shader.h>

Engine::Graphics::Material::Material(Texture * i_diffuseTexture, Texture * i_specularTexture, const float i_shininess):
	diffuseTexture(i_diffuseTexture), specularTexture(i_specularTexture), shininess(i_shininess)
{
	if (diffuseTexture) {
		diffuseTexture->IncrementReferenceCount();
	}

	if (specularTexture) {
		specularTexture->IncrementReferenceCount();
	}
}

Engine::Graphics::Material * Engine::Graphics::Material::CreateMaterial(Texture * diffuseTexture, Texture * specularTexture, const float shininess)
{
	return new Material(diffuseTexture, specularTexture, shininess);
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
		shader->SetInt("material.diffuse", diffuseTexture->GetTextureUnit());
		shader->SetBool("material.hasDiffuse", true);
		diffuseTexture->Bind();
	}
	else {
		shader->SetBool("material.hasDiffuse", false);
	}

	if (specularTexture) {
		shader->SetInt("material.specular", specularTexture->GetTextureUnit());
		shader->SetFloat("material.shininess", shininess);
		shader->SetBool("material.hasSpecular", true);
		specularTexture->Bind();
	}
	else {
		shader->SetBool("material.hasSpecular", false);
	}
}

Engine::Graphics::Material::~Material()
{
	if (diffuseTexture && !diffuseTexture->DecrementReferenceCount()) {
		Texture::DestroyTexture(diffuseTexture);
	}

	if (specularTexture && !specularTexture->DecrementReferenceCount()) {
		Texture::DestroyTexture(specularTexture);
	}
}
