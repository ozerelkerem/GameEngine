#include "ActorManager.h"



ActorManager::ActorManager(ComponentManager* cmi) : componentManagerInstance(cmi)
{
	this->actorContainer = new ActorContainer();
}


ActorManager::~ActorManager()
{
}

ActorID ActorManager::AqcuireActorID(Actor * actor)
{
	return this->actorHandleTable.AqcuireHandle(actor);
}

void ActorManager::ReleaseActorID(ActorID id)
{
	this->actorHandleTable.ReleaseHandle(id);
}
