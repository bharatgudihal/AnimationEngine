#include "SpotLight.h"

Engine::Lighting::SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Actor * actor, glm::vec3 i_direction, float cutOffRadians) :
	Light(ambient, diffuse, specular, actor), direction(direction), cutOff(cutOffRadians)
{
}

Engine::Lighting::SpotLight::~SpotLight()
{
}

void Engine::Lighting::SpotLight::Draw()
{
	if (showMesh && actor) {
		actor->Draw();
	}
}