#pragma once
#include <engine/components/Component.h>

class CameraComponent :public Component<CameraComponent>
{
public:
	CameraComponent(ActorID own, float fov, float aspect, float near, float far);
	~CameraComponent();

	float fov;
	float aspect;

	float near;
	float far;


	inline virtual IComponent * getnew(ActorID own, ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		auto x = ((IComponent*) new CameraComponent(*this));
		x->owner = own;
		return x;
	}
};

