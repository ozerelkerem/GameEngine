#pragma once

#include <engine/systems/System.h>
#include<physx/PxPhysicsAPI.h>
#include<engine/physx/CustomPhysxEventCallBack.h>
#include<util/MatConverter.h>
#include <engine/components/RigidBodyComponent.h>
#include <engine/components/colliders/CubeColliderComponent.h>
#include<engine/Transform.h>

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

using namespace physx;
class PhysicSystem : public System<PhysicSystem>
{
public:
	PhysicSystem();
	~PhysicSystem();

	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;

	Transform* getfix(ActorID actorid); // to solve circuler depency problem
	
	template<class T>
	 void addComponent(T* component);

private:
	/**/
	//false dynamic // true static
	std::unordered_map<ActorID::value_type, PxRigidActor*> objects;

	physx::PxDefaultAllocator		gAllocator;
	physx::PxDefaultErrorCallback	gErrorCallback;
	physx::PxFoundation		        *gFoundation = NULL;
	physx::PxPhysics*				gPhysics = NULL;
	physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
	physx::PxScene*				gScene = NULL;
	physx::PxPvd*                  gPvd = NULL;
	CustomPhysxEventCallBack*      gEventManager;
	void initPhysx();


};
template<class T>
void PhysicSystem::addComponent(T* component)
{
	auto it = objects.find((ActorID::value_type)component->owner);
	Transform* tr = getfix(component->owner);
	PxTransform t(glmMat4ToPhysxMat4(tr->getWorldMatrix()));

	if constexpr (std::is_same<T, RigidBodyComponent>::value)
	{//dynamic
		if (it == objects.end()) //new one
		{
			physx::PxRigidDynamic* dynamic = gPhysics->createRigidDynamic(t);
			dynamic->setAngularDamping(0.5f);
			dynamic->setLinearVelocity({ 1,0,0 });
			objects[component->owner] = dynamic;
		}
		else
		{
			PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*1);
			std::cout << it->second->getNbShapes();
			(it->second)->getShapes(shapes, 1);
			it->second->detachShape(*shapes[0]);
			gScene->removeActor(*(it->second));
			it->second = gPhysics->createRigidDynamic(t);
			it->second->attachShape(*shapes[0]);
			free(shapes);
		}
	}
	else
	{//nodynamic
		if (it == objects.end()) //new one
		{
			objects[component->owner] = gPhysics->createRigidStatic(t);
		}
		else
		{
			gScene->removeActor(*(it->second));
			it->second = gPhysics->createRigidDynamic(t);
		}
		objects[component->owner]->attachShape(*component->shape);
	}
	gScene->addActor(*objects[component->owner]);
	tr->physicactor = objects[component->owner];

	objects[component->owner]->userData = &component->owner;

}


