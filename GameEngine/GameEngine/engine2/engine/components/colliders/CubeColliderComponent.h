#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
#include<physx/PxPhysicsAPI.h>
class CubeColliderComponent : public ICollider, public Component<CubeColliderComponent>
{
public:
	CubeColliderComponent();
	~CubeColliderComponent();

	physx::PxBoxGeometry geometry;

	inline virtual void update() override { shape->setGeometry(geometry); };
};
