#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	eRotation = glm::vec3(0, 0, 0);
	qRotation = glm::toQuat((glm::orientate3(eRotation)));

	calcModelMatrix();
}

void Transform::calcEulerFromQuat()
{
	eRotation = glm::eulerAngles(qRotation);
}

void Transform::calcQuatFromEuler()
{
	qRotation = glm::toQuat((glm::orientate3(eRotation)));
}

inline void Transform::calcModelMatrix()
{
	modelMatrix = glm::scale( glm::translate(glm::mat4(1), position) * (glm::orientate4(eRotation)), scale);
}

Transform::~Transform()
{
}