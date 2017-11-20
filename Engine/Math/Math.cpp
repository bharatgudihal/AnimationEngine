#include "Math.h"

glm::mat4 Engine::Graphics::Math::CalculateTransform(const Transform & transform)
{
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
	trans = glm::rotate(trans, transform.rotationAngle, glm::vec3(transform.rotationAxis.x, transform.rotationAxis.y, transform.rotationAxis.z));
	trans = glm::scale(trans, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
	return trans;
}
