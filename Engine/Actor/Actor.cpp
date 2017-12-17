#include "Actor.h"
#include <Engine/Mesh/Mesh.h>
#include <Engine/Shader/Shader.h>
#include <Engine/Texture/Texture.h>

Engine::Actor::Actor(Graphics::Mesh * i_mesh, Graphics::Shader * i_shader):mesh(i_mesh), shader(i_shader)
{
}

Engine::Actor::~Actor()
{
}

void Engine::Actor::Draw()
{
	if (texture1) {
		texture1->Bind();
	}
	if (texture2) {
		texture2->Bind();
	}
	shader->Use();
	mesh->Draw();
}

void Engine::Actor::SetTexture1(Graphics::Texture * texture)
{
	shader->Use();
	shader->SetBool("useTexture1", true);
	shader->SetInt("texture1", 0);
	texture1 = texture;
}

void Engine::Actor::SetTexture2(Graphics::Texture * texture)
{
	shader->Use();
	shader->SetBool("useTexture2", true);
	shader->SetInt("texture2", 1);
	shader->SetFloat("blendRatio", 0.2f);
	texture2 = texture;
}
