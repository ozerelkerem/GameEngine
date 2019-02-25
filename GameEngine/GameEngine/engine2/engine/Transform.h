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

	void divideRealMatrix();

	glm::vec3 position;
	glm::quat qRotation;
	glm::vec3 eRotation;
	glm::vec3 scale;
	
	glm::mat4 relativeMatrix;
	glm::mat4 realMatrix;

	void calcEulerFromQuat();
	void calcQuatFromEuler();

	inline void calcModelMatrix();
	~Transform();
};
