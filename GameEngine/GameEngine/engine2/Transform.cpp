#include "Transform.h"

#define GLM_FORCE_

#define pi (float)3.14159

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	eRotation = glm::vec3(0, 0, 0);
	qRotation = glm::toQuat((glm::orientate3(eRotation)));

	realMatrix = glm::mat4(1);
	relativeMatrix = glm::mat4(1);
}

void Transform::divideRealMatrix()
{
	glm::vec3 skew(1);
	glm::vec4 perspective(1);
	glm::decompose(relativeMatrix, scale, qRotation, position, skew, perspective);
	calcEulerFromQuat();
}

void Transform::calcEulerFromQuat()
{
	eRotation = glm::eulerAngles(qRotation);
	eRotation = (180 / pi) * eRotation;
	
}

void Transform::calcQuatFromEuler()
{
	qRotation = glm::toQuat((glm::orientate3(glm::radians(glm::vec3(eRotation.x, eRotation.z, eRotation.y)))));
}

inline void Transform::calcModelMatrix()
{
	relativeMatrix = glm::scale(glm::translate(glm::mat4(1), position) * (glm::toMat4(qRotation)), scale);
}

Transform::~Transform()
{
}