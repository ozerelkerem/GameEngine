#pragma once

#include "../includes/glm/glm.hpp"
#include "../includes/glm/gtc/matrix_transform.hpp"
#include "../includes/GL/glew.h"

#include <vector>

class SceneCamera
{
private:
	glm::vec3 position;
	glm::vec3 targetPosition;
	glm::vec3 up;

	float cameraSpeed;

	glm::mat4x4 viewMatrix;
public:
	SceneCamera(glm::vec3, glm::vec3);
	void UpdateViewMatrix();
	glm::mat4x4 getViewMatrix();

	void MoveForward();
	void MoveBackward();
};

