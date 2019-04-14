#include "IModelComponent.h"





IModelComponent::~IModelComponent()
{
	GE_Engine->resourceManager->removeResource(this->model);
}
