#include "Transform.h"

#define GLM_FORCE_

#define pi (float)3.14159

Transform::Transform() : localPosition(0,0,0), localScale(1,1,1), localeulerRotation(0,0,0), worldMatrix(1), localMatrix(1)
{
	localquatRotation = glm::toQuat((glm::orientate3(localeulerRotation)));
}

void Transform::decomposeLocalMatrix()
{
	glm::vec3 skew(1);
	glm::vec4 perspective(1);
	glm::decompose(localMatrix, localScale, localquatRotation, localPosition, skew, perspective);
	calcEulerFromQuat();
}

void Transform::calcEulerFromQuat()
{
	localeulerRotation = glm::eulerAngles(localquatRotation);
	localeulerRotation = (180 / pi) * localeulerRotation;

}

void Transform::calcQuatFromEuler()
{
	localquatRotation = glm::toQuat((glm::orientate3(glm::radians(glm::vec3(localeulerRotation.x, localeulerRotation.z, localeulerRotation.y)))));
}

glm::vec3 Transform::getWorldPosition()
{
	glm::vec3 pos;
	glm::vec4 test;
	glm::quat testq;
	glm::vec3 testp;
	glm::decompose(worldMatrix, testp, testq, pos, testp, test);
	return pos;
}

glm::vec3 Transform::getWorldForwardVector()
{
	const glm::mat4 inverted = glm::inverse(worldMatrix);
	const glm::vec3 forward = normalize(glm::vec3(inverted[2]));
	return forward;
}
glm::vec3 Transform::getWorldUpVector()
{

	glm::vec3 up = glm::normalize(glm::vec3(worldMatrix[0][1], worldMatrix[1][1], worldMatrix[2][1]));

	return up;
}
glm::vec3 Transform::getWorldRightVector()
{
	glm::vec3 Right = glm::normalize(glm::vec3(worldMatrix[0][0], worldMatrix[1][0], worldMatrix[2][0]));
	return Right;
}

void Transform::calcLocalMatrix()
{
	localMatrix = glm::scale(glm::translate(glm::mat4(1), localPosition) * (glm::toMat4(localquatRotation)), localScale);
}

Transform::~Transform()
{
}