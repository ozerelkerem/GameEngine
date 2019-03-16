#pragma once
#include <engine/components/Component.h>

class CameraComponent :public Component<CameraComponent>
{
public:
	CameraComponent(float fov, float aspect, float near, float far);
	~CameraComponent();

	float fov;
	float aspect;

	float near;
	float far;


	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new CameraComponent(*this));
	}
};

