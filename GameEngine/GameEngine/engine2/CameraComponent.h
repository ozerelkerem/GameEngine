#pragma once
#include <Component.h>

class CameraComponent :public Component
{
public:
	CameraComponent(float fov, float aspect, float near, float far);
	~CameraComponent();

	float fov;
	float aspect;

	float near;
	float far;

	ComponentType getType() { return ComponentType::Camera; }
	Component * copy() { return new CameraComponent(*this); }
};

