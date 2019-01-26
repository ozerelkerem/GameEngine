#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	eRotation = glm::vec3(0, 0, 0);
	qRotation = glm::toQuat((glm::orientate3(eRotation)));
}

void Transform::calcEulerFromQuat()
{
	eRotation = glm::eulerAngles(qRotation);
}

void Transform::calcQuatFromEuler()
{
	qRotation = glm::toQuat((glm::orientate3(eRotation)));
}

Transform::~Transform()
{
}