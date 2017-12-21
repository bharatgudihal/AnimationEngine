#include "PointLight.h"
#include <Engine/Actor/Actor.h>
#include <Engine/Shader/Shader.h>
#include <glad\glad.h>

Engine::Lighting::PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor * actor):
	Light(ambient, diffuse, specular, actor)
{
}

Engine::Lighting::PointLight::~PointLight()
{
}

void Engine::Lighting::PointLight::Draw()
{
	if (showMesh && actor) {
		actor->Draw();
	}
}