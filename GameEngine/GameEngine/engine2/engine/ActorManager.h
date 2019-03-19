#pragma once
#include<Api.h>
#include<util/Handle.h>
#include<engine/Actor.h>

class ActorManager
{
public:

	using ActorHandleTable = GameEngine::Util::HandleTable<Actor, ActorID>;

	ActorHandleTable actorHandleTable;

	ActorManager();
	~ActorManager();

	ActorID AqcuireActorID(Actor* actor);
	void ReleaseActorID(ActorID id);


	inline Actor* GetActor(ActorID entityId)
	{
		return this->actorHandleTable[entityId];
	}

	inline ActorID GetActorID(ActorID::value_type index) const
	{
		return this->actorHandleTable[index];
	}

	template<class... ARGS>
	ActorID CreateActor(ARGS&&... args)
	{
		// aqcuire memory for new entity object of type T
		void* pObjectMemory = malloc(sizeof(Actor));
		ActorID actorid = this->AqcuireActorID((Actor*)pObjectMemory);
		Actor* actor = new (pObjectMemory)Actor(std::forward<ARGS>(args)...,actorid);

	

//		actor->actorID = actorid;
		//((T*)pObjectMemory)->m_ComponentManagerInstance = this->m_ComponentManagerInstance;

		// create entity inplace
		

		return actorid;
	}

	



};

