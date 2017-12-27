#include "Actor.h"
#include <Engine/Mesh/Mesh.h>
#include <Engine/Shader/Shader.h>
#include <Engine/Texture/Texture.h>
#include <Engine/Math/Math.h>
#include <Engine/Material/Material.h>

Engine::Actor::Actor(Graphics::Mesh * i_mesh):mesh(i_mesh), material(nullptr)
{
	mesh->IncrementReferenceCount();
}

Engine::Actor::~Actor()
{
	Graphics::Mesh::DestroyMesh(mesh);
}

void Engine::Actor::Draw(Graphics::Shader* shader)
{	
	shader->Use();
	shader->SetMatrix("model", Engine::Math::CalculateTransform(transform));
	if (material) {
		material->Bind(shader);
	}
	mesh->Draw();
}
