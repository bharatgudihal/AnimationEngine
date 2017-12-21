#include "Light.h"

Engine::Lighting::Light::Light(glm::vec3 i_ambient, glm::vec3 i_diffuse, glm::vec3 i_specular, Engine::Actor* i_actor) :
	ambient(i_ambient), diffuse(i_diffuse), specular(i_specular), actor(i_actor), showMesh(false)
{
}
