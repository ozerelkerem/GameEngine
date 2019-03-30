#include "Engine.h"

#include <engine/ActorManager.h>
#include <engine/resource/ResourceManager.h>
#include<engine/components/ComponentManager.h>

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
}
