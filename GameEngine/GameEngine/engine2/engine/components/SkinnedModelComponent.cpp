#include "SkinnedModelComponent.h"



SkinnedModelComponent::SkinnedModelComponent(ActorID own, Model *m) : IModelComponent(m), Component(own)
{
}


SkinnedModelComponent::~SkinnedModelComponent()
{
}
