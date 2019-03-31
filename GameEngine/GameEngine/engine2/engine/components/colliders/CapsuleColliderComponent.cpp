#include "CapsuleColliderComponent.h"



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
}
