#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include <glm/gtx/matrix_decompose.hpp>



class SceneCamera
{


public:

	float yaw;
	float pitch;

	
	glm::vec3 rightVector;
	glm::vec3 frontVector;

	const glm::vec3 worldUpVector = {0,0,1};
	const glm::vec3 worldRightVector = { 1,0,0 };
	const glm::vec3 worldFrontVector = { 0,1,0 };



	glm::vec3 position;
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;
	glm::vec3 upVector;
	SceneCamera(glm::vec3, float, float);

	float cameraSpeed;
	float mouseSensivity;

	void UpdateCameraVectors();

	void UpdateViewMatrix();
	glm::mat4x4 getViewMatrix();

	void MoveForward();
	void MoveBackward();

	void MoveRight();
	void MoveLeft();

	void MoveUp(float);
	void MoveDown(float);

	void RotateYaw(float angle);
	void RotatePitch(float angle);


	glm::vec2 worldToScreen(glm::vec3 &worldPos, glm::vec2 &size);
	glm::vec3 screenToWorld(int x, int y, glm::vec3 &planeNormal, glm::vec2 & viewportSize);
};

