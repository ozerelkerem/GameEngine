#include "ModelComponent.h"



ModelComponent::ModelComponent(ActorID own, Model *m) : IModelComponent(m) ,Component(own)
{
	
}


ModelComponent::~ModelComponent()
{
}
