#include "CustomPhysxEventCallBack.h"
#include <Engine.h>
#include <engine/ActorManager.h>
#include<engine/components/ScriptComponent.h>
#include<engine/scripting/ScriptSystem.h>

CustomPhysxEventCallBack::CustomPhysxEventCallBack() 
{
}


CustomPhysxEventCallBack::~CustomPhysxEventCallBack()
{
}

void CustomPhysxEventCallBack::onContact(const physx::PxContactPairHeader & pairHeader, const physx::PxContactPair * pairs, physx::PxU32 nbPairs)
{
	contacts.push_back(std::make_pair(pairHeader.actors[0],pairHeader.actors[1]));
}

void CustomPhysxEventCallBack::onTrigger(physx::PxTriggerPair * pairs, physx::PxU32 count)
{
	std::cout << " trigger here";
}

void CustomPhysxEventCallBack::safeupdate()
{
	if (!GE_Engine->scriptSystem->hasScripts())
	{
		contacts.clear();
		return;
	}
		
	for (auto contact : contacts)
	{
		Actor *a1 = GE_Engine->actorManager->GetActor(*(ActorID*)(contact.first->userData));
		Actor *a2 = GE_Engine->actorManager->GetActor(*(ActorID*)(contact.second->userData));
		MonoObject *ma1 = GE_Engine->scriptSystem->getActorObject(a1->actorID);
		MonoObject *ma2 = GE_Engine->scriptSystem->getActorObject(a2->actorID);

		ScriptComponent *a1sc = a1->GetComponent<ScriptComponent>();
		ScriptComponent *a2sc = a2->GetComponent<ScriptComponent>();

		void *params[1];
		params[0] = ma2;

		int i = 0;
		if (a1sc)
			for (auto script : a1sc->scripts)
			{
				mono_runtime_invoke(script->OnCollisionEnterMethod, a1sc->objects[i++], params, NULL);
			}
		i = 0;
		params[0] = ma1;
		if (a2sc)
			for (auto script : a2sc->scripts)
			{
				mono_runtime_invoke(script->OnCollisionEnterMethod, a2sc->objects[i++], params, NULL);
			}
	}
	contacts.clear();
}
