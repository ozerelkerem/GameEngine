#include "CameraComponent.h"



CameraComponent::CameraComponent(ActorID own, float fov, float aspect, float near, float far) : Component(own)
{
	this->aspect = aspect;
	this->m_far = far;
	this->fov = fov;
	this->m_near = near;
}


CameraComponent::~CameraComponent()
{
}
