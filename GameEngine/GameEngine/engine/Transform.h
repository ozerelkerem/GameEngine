#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtx/euler_angles.hpp"
#include "../ogl/glm/gtx/quaternion.hpp"
#include "../ogl/glm/gtx/transform.hpp"

class Transform
{
public:
	glm::vec3 position;
	glm::quat qRotation;
	glm::vec3 eRotation;
	glm::vec3 scale;

	glm::mat4 localMatrix;
	glm::mat4 modelMatrix;

	Transform();

	void calcEulerFromQuat();
	void calcQuatFromEuler();


	inline void calcModelMatrix();
	~Transform();
};
