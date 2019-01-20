#pragma once 

#include "SceneCamera.h"


SceneCamera::SceneCamera(glm::vec3 position, glm::vec3 targetPosition)
{
	this->position = position;
	this->targetPosition = targetPosition;
	this->Update();
}

void SceneCamera::Update()
{
	viewMatrix = glm::lookAt(this->position, this->targetPosition, glm::vec3(0, 1, 0));
}

glm::mat4 SceneCamera::getViewMatrix()
{
	return viewMatrix;
}