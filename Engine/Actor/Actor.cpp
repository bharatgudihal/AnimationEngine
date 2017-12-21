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
	shader->Use();
	mesh->Draw();
}
