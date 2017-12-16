#include "Math.h"

glm::mat4 Engine::Math::CalculateTransform(const Transform & transform)
{
	glm::mat4 trans;
	trans = glm::translate(trans, transform.position);
	trans = glm::rotate(trans, transform.rotationAngle, transform.rotationAxis);
	trans = glm::scale(trans, transform.scale);
	return trans;
}