#include "SimpleLight.h"
#include <Engine/Actor/Actor.h>
#include <Engine/Shader/Shader.h>
#include <glad\glad.h>

Engine::Lighting::SimpleLight::SimpleLight(glm::vec3 i_ambient, glm::vec3 i_diffuse, glm::vec3 i_specular, Engine::Actor* i_actor):
	ambient(i_ambient),diffuse(i_diffuse), specular(i_specular), actor(i_actor), showMesh(false)
{
}

Engine::Lighting::SimpleLight::~SimpleLight()
{
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