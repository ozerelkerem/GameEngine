#pragma once
#include <engine/components/Component.h>


class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent();
	virtual ~RigidBodyComponent();

	physx::PxRigidDynamic *pxactor;
	
	void addForce(physx::PxVec3 forceVec);

};

