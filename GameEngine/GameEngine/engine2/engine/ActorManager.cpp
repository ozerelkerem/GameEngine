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

void ActorManager::RemoveDestroyedActors()
{
	for (size_t i = 0; i < this->numPendingDestroyedActors; ++i)
	{
		ActorID actorid = this->pendingDestroyedActors[i];

		Actor* actor = this->actorHandleTable[actorid];

		

	
	
			// release entity's components
			this->componentManagerInstance->RemoveAllComponents(actorid);

			actorContainer->DestroyActor(actor);
	

		// free entity id
		this->ReleaseActorID(actorid);
	}

	this->numPendingDestroyedActors = 0;
}
