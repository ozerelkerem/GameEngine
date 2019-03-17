#include "ComponentSystem.h"
#include<engine/Actor.h>
ComponentSystem::ComponentSystem()
{
}

ComponentSystem::~ComponentSystem()
{
}


//@TODO ADD LIGHT COMPONENT
void ComponentSystem::addActor(Actor *a)
{
	if (a->componentObject->hasComponent<ModelComponent>())
	{
		auto comp = a->componentObject->getComponent<ModelComponent>();
		AddComponent(a, comp);
	}
	if (a->componentObject->hasComponent<LightComponent>())
	{
		auto comp = a->componentObject->getComponent<LightComponent>();
		AddComponent(a, comp);
	}
	if (a->componentObject->hasComponent<SkinnedModelComponent>())
	{
		auto comp = a->componentObject->getComponent<SkinnedModelComponent>();
		AddComponent(a, comp);
	}
	if (a->componentObject->hasComponent<AnimatorComponent>())
	{
		auto comp = a->componentObject->getComponent<AnimatorComponent>();
		AddComponent(a, comp);
	}

	
}

template<>
void ComponentSystem::changeModel<ModelComponent>(Actor *actor, Model *newmodel)
{
	actorsWhichContainsModelComponent[actor->componentObject->getComponent<ModelComponent>()->model].remove(actor);
	actorsWhichContainsModelComponent[newmodel].push_front(actor);
}
template<>
void ComponentSystem::changeModel<SkinnedModelComponent>(Actor *actor, Model *newmodel)
{
	actorsWhichContainsModelComponent[actor->componentObject->getComponent<SkinnedModelComponent>()->model].remove(actor);
	actorsWhichContainsModelComponent[newmodel].push_front(actor);
}



