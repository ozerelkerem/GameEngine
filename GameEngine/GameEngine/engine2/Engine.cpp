#include "Engine.h"

#include <engine/ActorManager.h>
#include <engine/resource/ResourceManager.h>
Engine::Engine()
{
	actorManager = new ActorManager();
	resourceManager = new ResourceManager();
}


Engine::~Engine()
{
}

void Engine::Update()
{
	time = std::chrono::high_resolution_clock::now();
}
