#include "CapsuleColliderComponent.h"
#include<engine/physx/PhysicSystem.h>



CapsuleColliderComponent::CapsuleColliderComponent()
{
	upp = 0;
	geometry.radius = 1.f;
	geometry.halfHeight = 1.f;
	shape = PxGetPhysics().createShape(geometry, *ConstantPhysicMaterials::PhysicMaterials::defaultMaterial, true);
	update();
}


CapsuleColliderComponent::~CapsuleColliderComponent()
{
	GE_Engine->physicSystem->removeComponent(this);
}
