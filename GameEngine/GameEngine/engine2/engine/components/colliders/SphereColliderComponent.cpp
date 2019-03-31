#include "SphereColliderComponent.h"



SphereColliderComponent::SphereColliderComponent()
{
	geometry.radius = 1.f;
	shape = PxGetPhysics().createShape(geometry, *ConstantPhysicMaterials::PhysicMaterials::defaultMaterial, true);
}


SphereColliderComponent::~SphereColliderComponent()
{
}
