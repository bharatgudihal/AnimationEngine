#include "SimpleLight.h"
#include <Engine/Actor/Actor.h>
#include <Engine/Shader/Shader.h>

Engine::Lighting::SimpleLight::SimpleLight(glm::vec3 color, Engine::Actor* i_actor):color(color), actor(i_actor), showMesh(false)
{
}

Engine::Lighting::SimpleLight::~SimpleLight()
{
}

void Engine::Lighting::SimpleLight::Apply(Graphics::Shader * shader)
{
	shader->Use();
	shader->SetVector("lightColor", color);
	shader->SetVector("lightPos", actor->transform.position);
}

void Engine::Lighting::SimpleLight::ShowMesh(const bool showMesh)
{
	this->showMesh = showMesh;	
}

void Engine::Lighting::SimpleLight::Draw()
{
	if (showMesh && actor) {
		actor->Draw();
	}
}
