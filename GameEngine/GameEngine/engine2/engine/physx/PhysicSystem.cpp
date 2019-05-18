#include "PhysicSystem.h"

#include<engine/ActorManager.h>

PhysicSystem::PhysicSystem()
{
	initPhysx();
}
static PxFilterFlags filterShader(PxFilterObjectAttributes attributes0,
	PxFilterData filterData0,
	PxFilterObjectAttributes attributes1,
	PxFilterData filterData1,
	PxPairFlags& pairFlags,
	const void* constantBlock,
	PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;
}
PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;
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
	gEventManager->safeupdate();

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
	gEventManager = new CustomPhysxEventCallBack();
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.simulationEventCallback = gEventManager;
	sceneDesc.gravity = physx::PxVec3(0.0f, 0.0f, -9.81f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
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

