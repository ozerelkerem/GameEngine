#include "Engine.h"


#include<engine/components/ComponentManager.h>
#include <engine/ActorManager.h>
#include <engine/resource/ResourceManager.h>
#include <engine/systems/AnimationSystem.h>
#include <engine/physx/PhysicSystem.h>
#include <engine/scripting/ScriptSystem.h>
#include <engine/systems/SystemManager.h>

Engine::Engine()
{
	componentManager = new ComponentManager();
	actorManager = new ActorManager(componentManager);
	physicSystem = new PhysicSystem();
	scriptSystem = new ScriptSystem();
	resourceManager = new ResourceManager();
	animationSystem = new AnimationSystem();
	sytemManager = new SystemManager(animationSystem,physicSystem,scriptSystem);

	physicSystem->enabled = false;
	scriptSystem->enabled = false;
}


Engine::~Engine()
{
}

void Engine::Update()
{
	time = std::chrono::high_resolution_clock::now();
	actorManager->RemoveDestroyedActors();
}
