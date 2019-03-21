#pragma once

#include <chrono>

class ActorManager;

class Engine
{
public:
	std::chrono::time_point<std::chrono::high_resolution_clock> time;

	Engine();
	~Engine();

	void Update();

	inline std::chrono::time_point<std::chrono::high_resolution_clock> getTime() { return time; }

	ActorManager * actorManager;
};

