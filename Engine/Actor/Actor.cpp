#include "Actor.h"
#include <Engine/Mesh/Mesh.h>
#include <Engine/Shader/Shader.h>
#include <Engine/Texture/Texture.h>
#include <Engine/Math/Math.h>
#include <Engine/Material/Material.h>

Engine::Actor::Actor(std::vector<Graphics::Mesh*> i_meshes, std::vector<Graphics::Material*> i_materials):meshes(i_meshes), materials(i_materials)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->IncrementReferenceCount();
	}

	for (unsigned int i = 0; i < materials.size(); i++) {
		materials[i]->IncrementReferenceCount();
	}
}

Engine::Actor::~Actor()
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		Graphics::Mesh::DestroyMesh(meshes[i]);
	}

	for (unsigned int i = 0; i < materials.size(); i++) {
		Graphics::Material::DestroyMaterial(materials[i]);
	}
}

void Engine::Actor::Draw(Graphics::Shader* shader)
{	
	shader->Use();
	shader->SetMatrix("model", Engine::Math::CalculateTransform(transform));
	for (unsigned int i = 0; i < meshes.size(); i++) {
		if (i < materials.size()) {
			materials[i]->Bind(shader);
		}
		meshes[i]->Draw();
	}
}
