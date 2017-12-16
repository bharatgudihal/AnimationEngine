#include "SimpleLight.h"
#include <Engine/Mesh/Mesh.h>

Engine::Graphics::Lighting::SimpleLight::SimpleLight(glm::vec3 color):color(color), mesh(nullptr), showMesh(false)
{
}

Engine::Graphics::Lighting::SimpleLight::~SimpleLight()
{
	if(mesh){
		Mesh::DestroyMesh(mesh);
	}
}

void Engine::Graphics::Lighting::SimpleLight::ShowMesh(const bool showMesh)
{
	this->showMesh = showMesh;
	if (!mesh) {
		mesh = Engine::Graphics::Mesh::GetCube();
	}
}

void Engine::Graphics::Lighting::SimpleLight::Draw()
{
	if (showMesh) {
		mesh->Draw();
	}
}
