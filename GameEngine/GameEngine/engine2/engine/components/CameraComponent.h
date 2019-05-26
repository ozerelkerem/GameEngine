#pragma once
#include<Api.h>
#include <engine/components/Component.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glm/gtx/matrix_decompose.hpp>
class CameraComponent :public Component<CameraComponent>
{
public:
	CameraComponent();
	~CameraComponent();

	bool Perspective;

	glm::mat4x4 projectionMatrix;
	glm::mat4x4 viewMatrix;

	

	float fov;
	float m_near;
	float m_far;
	float size;


	void updateViewMatrix();
	void updateprojectionMatrix();
};

