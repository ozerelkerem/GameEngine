#include "CameraComponent.h"



CameraComponent::CameraComponent(float fov, float aspect, float near, float far) : Component()
{
	this->aspect = aspect;
	this->m_far = far;
	this->fov = fov;
	this->m_near = near;
}


CameraComponent::~CameraComponent()
{
}
