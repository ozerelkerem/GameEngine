#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtx/euler_angles.hpp"
#include "../ogl/glm/gtx/quaternion.hpp"
#include "../ogl/glm/gtx/transform.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class Transform
{
public:
	Transform();

	void decomposeLocalMatrix();

	glm::vec3 localPosition;
	glm::quat localquatRotation;
	glm::vec3 localeulerRotation;
	glm::vec3 localScale;
	
	glm::mat4 localMatrix;
	glm::mat4 worldMatrix;

	void calcEulerFromQuat();
	void calcQuatFromEuler();

	glm::vec3 getWorldPosition();

	void calcLocalMatrix();
	~Transform();
};
