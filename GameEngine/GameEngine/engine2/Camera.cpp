#include "Camera.h"



Camera::Camera(float fov, float aspect, float near, float far, std::string name) : Object(name)
{
	this->aspect = aspect;
	this->far = far;
	this->fov = fov;
	this->near = near;
}


Camera::~Camera()
{
}
