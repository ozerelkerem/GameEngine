#pragma once
#include <engine/components/Component.h>

using namespace physx;
class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent();
	virtual ~RigidBodyComponent();

	physx::PxRigidDynamic *pxactor;
	


	bool LLX, LLY, LLZ, LRX, LRY, LRZ;

	bool Kinematic;
	bool Gravity;

	void updateFlags();
	void clearForces();
	
	void addForce(PxVec3 forceVec);
	void addTorque(PxVec3 torqueVec);
	void clearForce();
	void clearTorque();
	void setAngularDamping(PxReal angDamping);
	void setAngularVelocity(PxVec3 angVelocity);
	void setLinearDamping(PxReal linDamping);
	void setLinearVelocity(PxVec3 linVelocity);
	void setMass(PxReal mass);
	void setMaxLinearVelocity(PxReal maxLinVelocity);
	void setMaxAngularVelocity(PxReal maxAngVelocity);
	PxReal getAngularDamping();
	PxVec3 getAngularVelocity();
	PxReal getLinearDamping();
	PxVec3 getLinearVelocity();
	PxReal getMass();
	PxReal getMaxLinearVelocity();
	PxReal getMaxAngularVelocity();


};

