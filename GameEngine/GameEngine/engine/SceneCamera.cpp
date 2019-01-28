#pragma once 

#include "SceneCamera.h"

SceneCamera::SceneCamera(glm::vec3 position, float yaw, float pitch)
{
	this->position = position;
	this->yaw = yaw;
	this->pitch = pitch;

	UpdateCameraVectors();

	cameraSpeed = 1;
	mouseSensivity = 1;

	this->UpdateViewMatrix();
}

void SceneCamera::UpdateCameraVectors()
{
	frontVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVector.z = sin(glm::radians(pitch));
	frontVector.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	frontVector = glm::normalize(frontVector);

	rightVector = glm::normalize(glm::cross(frontVector, worldUpVector));
	upVector = glm::normalize(glm::cross(rightVector, frontVector));
}

void SceneCamera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(this->position, this->position + frontVector, upVector);
}

void SceneCamera::MoveForward()
{
	glm::vec3 direction = frontVector * cameraSpeed;
	position += direction;

	this->UpdateViewMatrix();
}

void SceneCamera::MoveBackward()
{
	glm::vec3 direction = frontVector * cameraSpeed;
	position -= direction;

	this->UpdateViewMatrix();
}

void SceneCamera::MoveRight()
{
	position -= rightVector * cameraSpeed;

	UpdateViewMatrix();
}

void SceneCamera::MoveLeft()
{
	position += rightVector * cameraSpeed;

	UpdateViewMatrix();
}

void SceneCamera::MoveUp(float sens = 1)
{
	position += worldUpVector * cameraSpeed * sens;

	UpdateViewMatrix();
}
void SceneCamera::MoveDown(float sens = 1)
{
	position -= worldUpVector * cameraSpeed * sens;

	UpdateViewMatrix();
}

void SceneCamera::RotateYaw(float angle)
{
	yaw -= angle * mouseSensivity;

	UpdateCameraVectors();
	UpdateViewMatrix();
}

void SceneCamera::RotatePitch(float angle)
{
	pitch += angle * mouseSensivity;

	if (pitch < -89)
		pitch = -89;
	else if (pitch > 89)
		pitch = 89;

	UpdateCameraVectors();
	UpdateViewMatrix();
}

glm::vec2 SceneCamera::worldToScreen(glm::vec3 & worldPos, glm::vec2 &size)
{
	glm::vec4 pos = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1);
	glm::vec3 pos2(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w);

	glm::vec2 returnVector(((pos2.x + 1.0) / 2.0) * size.x, ((pos2.y + 1.0) / 2.0) * size.y);

	return returnVector;
}

glm::mat4 SceneCamera::getViewMatrix(){ return viewMatrix; }