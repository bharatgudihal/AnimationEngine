#include "Material.h"
#include <Engine/Texture/Texture.h>
#include <Engine/Shader/Shader.h>

Engine::Graphics::Material::Material(Texture * i_diffuseTexture, Texture * i_specularTexture, const float i_shininess):
	diffuseTexture(i_diffuseTexture), specularTexture(i_specularTexture), shininess(i_shininess)
{
}

void Engine::Graphics::Material::Bind(Shader* shader)
{
	shader->Use();
	shader->SetInt("material.diffuse", diffuseTexture->GetTextureUnit());
	shader->SetInt("material.specular", specularTexture->GetTextureUnit());
	shader->SetFloat("material.shininess", shininess);
	diffuseTexture->Bind();
	specularTexture->Bind();
}

Engine::Graphics::Material::~Material()
{
}
