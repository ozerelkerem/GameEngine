#pragma once 

#include "SceneCamera.h"

SceneCamera::SceneCamera(glm::vec3 position, glm::vec3 targetPosition)
{
	this->position = position;
	this->targetPosition = targetPosition;

	cameraSpeed = 2;

	this->UpdateViewMatrix();
}

void SceneCamera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(this->position, this->targetPosition, glm::vec3(0, 0, 1));
}

void SceneCamera::MoveForward()
{
	glm::vec3 direction = targetPosition - position;

	direction = glm::normalize(direction) * cameraSpeed;

	targetPosition += direction;
	position += direction;

	this->UpdateViewMatrix();
}

void SceneCamera::MoveBackward()
{
	glm::vec3 direction = targetPosition - position;

	direction = glm::normalize(direction) * cameraSpeed;

	targetPosition -= direction;
	position -= direction;

	this->UpdateViewMatrix();
}

glm::mat4 SceneCamera::getViewMatrix(){ return viewMatrix; }