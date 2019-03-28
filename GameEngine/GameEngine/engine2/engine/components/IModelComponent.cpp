#include "IModelComponent.h"

#include <editor/ConstantModels.h>

IModelComponent::IModelComponent(Model *m)
{
	model = m;
	numberOfMaterials = m->numOfMeshes;
	materials.resize(numberOfMaterials,ConstantMaterials::Materials::defaultMaterial);

}


IModelComponent::~IModelComponent()
{
}
