#include "RigidBodyComponent.h"
#include<engine/physx/PhysicSystem.h>


RigidBodyComponent::RigidBodyComponent()
{
}


RigidBodyComponent::~RigidBodyComponent()
{
	GE_Engine->physicSystem->removeComponent(this);
}
