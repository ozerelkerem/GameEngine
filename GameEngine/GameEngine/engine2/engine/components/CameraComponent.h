#pragma once
#include <engine/components/Component.h>

class CameraComponent :public Component<CameraComponent>
{
public:
	CameraComponent(float fov, float aspect, float near, float far);
	~CameraComponent();

	float fov;
	float aspect;

	float m_near;
	float m_far;



};

