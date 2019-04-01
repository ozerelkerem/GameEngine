#include "CubeColliderComponent.h"



CubeColliderComponent::CubeColliderComponent() 
{
	geometry.halfExtents.x = 1;
	geometry.halfExtents.y = 2;
	geometry.halfExtents.z = 1;
	shape = PxGetPhysics().createShape(geometry, *ConstantPhysicMaterials::PhysicMaterials::defaultMaterial, true);
}


CubeColliderComponent::~CubeColliderComponent()
{
}