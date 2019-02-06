#pragma once

#include <Object.h>

class Camera : public Object
{
public:
	Camera(float fov, float aspect, float near, float far, std::string name);
	~Camera();

	float fov;
	float aspect;

	float near;
	float far;
};

