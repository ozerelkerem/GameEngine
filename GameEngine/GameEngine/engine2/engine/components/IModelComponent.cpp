#include "IModelComponent.h"



IModelComponent::IModelComponent(Model *m)
{
	model = m;
	numberOfMaterials = m->numOfMeshes;
}


IModelComponent::~IModelComponent()
{
}
