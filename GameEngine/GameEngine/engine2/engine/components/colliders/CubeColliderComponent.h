#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
#include<physx/PxPhysicsAPI.h>
class CubeColliderComponent :  public Component<CubeColliderComponent>, public ICollider
{
public:
	CubeColliderComponent();
	virtual ~CubeColliderComponent();

	physx::PxBoxGeometry geometry;

	inline virtual void update() override { shape->setGeometry(geometry); };
};

