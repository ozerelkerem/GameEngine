#include "PhysicSystem.h"

#include<engine/ActorManager.h>

PhysicSystem::PhysicSystem()
{
	initPhysx();
}


PhysicSystem::~PhysicSystem()
{

	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}

void PhysicSystem::PreUpdate()
{
}

void PhysicSystem::Update()
{
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);

	for (auto obje : objects)
	{
		Actor *actor = GE_Engine->actorManager->GetActor(obje.first);
		glm::mat4 temp;
		PhysXMat4ToglmMat4(obje.second->getGlobalPose(), temp);
		temp = glm::scale(temp, actor->transformation.getWorldScale());
		actor->transformation.setWorldMatrix(static_cast<glm::mat4>(temp));
	}
}

void PhysicSystem::PostUpdate()
{
}

Transform * PhysicSystem::getfix(ActorID actorid)
{
	return &GE_Engine->actorManager->GetActor(actorid)->transformation;
}

void PhysicSystem::initPhysx()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, 0.0f, -9.81f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	


	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_DYNAMIC, 2.0f);
}

