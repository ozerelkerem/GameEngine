#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
class SphereColliderComponent : public ICollider, public Component<SphereColliderComponent>
{
public:
	SphereColliderComponent();
	~SphereColliderComponent();

	float radius;
};

