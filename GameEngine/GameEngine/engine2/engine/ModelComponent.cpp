#include "ModelComponent.h"



ModelComponent::ModelComponent(Model *m) : Component()
{
	model = m;
	numberOfMaterials = m->numOfMeshes;
}


ModelComponent::~ModelComponent()
{
}
