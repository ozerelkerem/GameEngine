#include <CameraComponent.h>



CameraComponent::CameraComponent(float fov, float aspect, float near, float far) : Component()
{
	this->aspect = aspect;
	this->far = far;
	this->fov = fov;
	this->near = near;
}


CameraComponent::~CameraComponent()
{
}
