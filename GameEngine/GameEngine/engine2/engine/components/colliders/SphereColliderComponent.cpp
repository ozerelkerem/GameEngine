#include "SphereColliderComponent.h"
#include<engine/physx/PhysicSystem.h>


SphereColliderComponent::SphereColliderComponent()
{
	geometry.radius = 1.f;
	shape = PxGetPhysics().createShape(geometry, *ConstantPhysicMaterials::PhysicMaterials::defaultMaterial, true);
}


SphereColliderComponent::~SphereColliderComponent()
{
	GE_Engine->physicSystem->removeComponent(this);
}
