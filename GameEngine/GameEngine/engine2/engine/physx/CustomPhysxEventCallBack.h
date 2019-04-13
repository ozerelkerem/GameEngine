#pragma once
#include <Api.h>
#include<physx/PxPhysicsAPI.h>

class CustomPhysxEventCallBack : public physx::PxSimulationEventCallback
{
	
public:
	 CustomPhysxEventCallBack();
	~CustomPhysxEventCallBack();

	 void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	 void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	 void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) { std::cout << "break"; }
	 void onWake(physx::PxActor**, physx::PxU32) { std::cout << "wake";  }
	 void onSleep(physx::PxActor**, physx::PxU32) { std::cout << "sleep"; }
	 void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) { std::cout << "advance"; }

	 std::vector<std::pair< physx::PxRigidActor* , physx::PxRigidActor*>> contacts;

	 void safeupdate();
};

