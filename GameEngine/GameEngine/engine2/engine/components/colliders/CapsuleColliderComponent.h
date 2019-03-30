#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
class CapsuleColliderComponent : public ICollider, public Component<CapsuleColliderComponent>
{
public:
	CapsuleColliderComponent();
	~CapsuleColliderComponent();

	float radius;
	float halfheight;
};

