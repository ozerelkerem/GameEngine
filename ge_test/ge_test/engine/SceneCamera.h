#pragma once

#include "../includes/glm/glm.hpp"
#include "../includes/glm/gtc/matrix_transform.hpp"

#include <vector>

class SceneCamera
{
private:
	glm::vec3 position;
	glm::vec3 targetPosition;
	glm::vec3 up;

	glm::mat4 viewMatrix;
public:
	SceneCamera(glm::vec3, glm::vec3);
	void Update();
	glm::mat4 getViewMatrix();
};

