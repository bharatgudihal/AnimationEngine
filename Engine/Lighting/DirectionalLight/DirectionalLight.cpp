#include "DirectionalLight.h"

Engine::Lighting::DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor * actor, glm::vec3 direction)
	: Light(ambient, diffuse, specular, actor), lightDirection(direction)
{
}

Engine::Lighting::DirectionalLight::~DirectionalLight()
{
}

void Engine::Lighting::DirectionalLight::Draw(Graphics::Shader* shader)
{
	if (showMesh && actor) {
		actor->Draw(shader);
	}
}
