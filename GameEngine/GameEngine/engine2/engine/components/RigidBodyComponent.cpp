#include "RigidBodyComponent.h"
#include<engine/physx/PhysicSystem.h>


RigidBodyComponent::RigidBodyComponent()
{
}


RigidBodyComponent::~RigidBodyComponent()
{
	GE_Engine->physicSystem->removeComponent(this);
}

void RigidBodyComponent::addForce(PxVec3 forceVec)
{
	pxactor->addForce(forceVec);
}
