#pragma once 

#include "SceneCamera.h"
#include<iostream>
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
	glm::vec3 returned = glm::project(worldPos, viewMatrix, projectionMatrix, glm::vec4(0,0,size.x,size.y));
	if (returned.z > 1)
		return glm::vec2(-1000, -1000);
	return glm::vec2(returned.x,returned.y);
}

glm::vec3 SceneCamera::screenToWorld(int x, int y, glm::vec3 &planeNormal, glm::vec2 &viewportSize)
{
	glm::vec3 nearSource((float)x, (float)y, 0.f);
	glm::vec3 farSource((float)x, (float)y, 1.f);
	glm::vec3 nearPoint = glm::unProject(nearSource, viewMatrix, projectionMatrix, glm::vec4(0, 0, viewportSize.x, viewportSize.y));
	glm::vec3 farPoint = glm::unProject(farSource, viewMatrix, projectionMatrix, glm::vec4(0, 0, viewportSize.x, viewportSize.y));

	// Create a ray from the near clip plane to the far clip plane.
	glm::vec3 direction = farPoint - nearPoint;
	direction = glm::normalize(direction);

	// Calculate distance of intersection point from r.origin.
	float denominator = glm::dot(planeNormal, direction);
	float numerator = glm::dot(planeNormal, nearPoint);
	float t = -(numerator / denominator);


	// Calculate the picked position on the y = 0 plane.
	glm::vec3 pickedPosition = nearPoint + direction * t;
	return pickedPosition;
	/**/
}

glm::mat4 SceneCamera::getViewMatrix(){ return viewMatrix; }