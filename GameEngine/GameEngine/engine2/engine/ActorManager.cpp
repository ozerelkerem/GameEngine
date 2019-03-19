#include "ActorManager.h"



ActorManager::ActorManager()
{
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
