#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
class SphereColliderComponent :  public Component<SphereColliderComponent>, public ICollider
{
public:
	SphereColliderComponent();
	virtual ~SphereColliderComponent();

	physx::PxSphereGeometry geometry;

	inline virtual void update() override { shape->setGeometry(geometry); };

};

