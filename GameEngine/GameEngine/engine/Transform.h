#pragma once

#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtc/quaternion.hpp"

class Transform
{
public:
	glm::vec3 position;
	glm::quat qRotation;
	glm::vec3 eRotation;
	glm::vec3 scale;

	glm::mat4 localMatrix;

	Transform();
	~Transform();
};

