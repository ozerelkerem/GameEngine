#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
class CapsuleColliderComponent : public Component<CapsuleColliderComponent>, public ICollider
{
public:
	CapsuleColliderComponent();
	virtual ~CapsuleColliderComponent();

	int upp;
	physx::PxVec3 up;
	physx::PxCapsuleGeometry geometry;

	inline virtual void update() override {
		if (upp == 0)
			up = { 0,1,0 };
		else if (upp == 1)
			up = { 1,0,0 };
		else
			up = { 0,0,1 };
		shape->setGeometry(geometry); 
		shape->setLocalPose(physx::PxTransform(physx::PxQuat(physx::PxHalfPi, up))); 
	};
};

