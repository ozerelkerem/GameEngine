#include "ModelComponent.h"

#include <editor/ConstantModels.h>

ModelComponent::ModelComponent(Model *m) : Component(), IModelComponent()
{
	setModel(m);
	numberOfMaterials = m->numOfMeshes;
	materials.resize(numberOfMaterials, ConstantMaterials::Materials::defaultMaterial);
}


ModelComponent::~ModelComponent()
{
}
