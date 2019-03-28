#include "ComponentSystem.h"
#include<engine/Actor.h>
#include<engine/ActorManager.h>
ComponentSystem::ComponentSystem()
{
}

ComponentSystem::~ComponentSystem()
{
}


//@TODO ADD LIGHT COMPONENT
void ComponentSystem::addActor(ActorID aid)
{
	Actor *a = GE_Engine->actorManager->GetActor(aid);
	if (a->componentObject->hasComponent<ModelComponent>())
	{
		auto comp = a->componentObject->getComponent<ModelComponent>();
		AddComponent(aid, comp);
	}
	if (a->componentObject->hasComponent<LightComponent>())
	{
		auto comp = a->componentObject->getComponent<LightComponent>();
		AddComponent(aid, comp);
	}
	if (a->componentObject->hasComponent<SkinnedModelComponent>())
	{
		auto comp = a->componentObject->getComponent<SkinnedModelComponent>();
		AddComponent(aid, comp);
	}
	if (a->componentObject->hasComponent<AnimatorComponent>())
	{
		auto comp = a->componentObject->getComponent<AnimatorComponent>();
		AddComponent(aid, comp);
	}

	
}

void ComponentSystem::removeActor(ActorID aid)
{
	Actor *actor = GE_Engine->actorManager->GetActor(aid);
	ModelComponent *mc = actor->componentObject->getComponent<ModelComponent>();
	SkinnedModelComponent *smc = actor->componentObject->getComponent<SkinnedModelComponent>();
	if (mc)
	{
		actorsWhichContainsModelComponent[mc->model].remove(aid);
		delete mc->model;
	}
	if (smc)
	{
		actorsWhichContainsSkinnedModelComponent[smc->model].remove(aid);
		delete smc->model;
	}
}

template<>
void ComponentSystem::changeModel<ModelComponent>(ActorID aid, Model *newmodel)
{
	Actor *actor = GE_Engine->actorManager->GetActor(aid);
	actorsWhichContainsModelComponent[actor->componentObject->getComponent<ModelComponent>()->model].remove(aid);
	actorsWhichContainsModelComponent[newmodel].push_front(aid);
}
template<>
void ComponentSystem::changeModel<SkinnedModelComponent>(ActorID aid, Model *newmodel)
{
	Actor *actor = GE_Engine->actorManager->GetActor(aid);
	actorsWhichContainsSkinnedModelComponent[actor->componentObject->getComponent<SkinnedModelComponent>()->model].remove(aid);
	actorsWhichContainsSkinnedModelComponent[newmodel].push_front(aid);
}



