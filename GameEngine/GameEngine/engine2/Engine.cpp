#include "Engine.h"

#include <engine/ActorManager.h>

Engine::Engine()
{
	actorManager = new ActorManager();
}


Engine::~Engine()
{
}

void Engine::Update()
{
	time = std::chrono::high_resolution_clock::now();
}
