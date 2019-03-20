#include "CameraComponent.h"



CameraComponent::CameraComponent(ActorID own, float fov, float aspect, float near, float far) : Component(own)
{
	this->aspect = aspect;
	this->far = far;
	this->fov = fov;
	this->near = near;
}


CameraComponent::~CameraComponent()
{
}
