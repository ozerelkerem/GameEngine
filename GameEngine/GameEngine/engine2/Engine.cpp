#include "Engine.h"


#include<engine/components/ComponentManager.h>
#include <engine/ActorManager.h>
#include <engine/resource/ResourceManager.h>


Engine::Engine()
{
	componentManager = new ComponentManager();
	actorManager = new ActorManager(componentManager);
	resourceManager = new ResourceManager();
}


Engine::~Engine()
{
}

void Engine::Update()
{
	time = std::chrono::high_resolution_clock::now();
	actorManager->RemoveDestroyedActors();
}
