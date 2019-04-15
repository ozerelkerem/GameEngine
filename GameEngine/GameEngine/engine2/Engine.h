#pragma once

#include <chrono>

class ActorManager;
class ResourceManager;
class ComponentManager;
class AnimationSystem;
class SystemManager;
class PhysicSystem;
class ScriptSystem;
class ProjectManager;

class Engine
{
public:
	std::chrono::time_point<std::chrono::high_resolution_clock> time;

	Engine();
	~Engine();

	void Update();

	inline std::chrono::time_point<std::chrono::high_resolution_clock> getTime() { return time; }

	AnimationSystem * animationSystem;
	PhysicSystem * physicSystem;
	SystemManager* sytemManager;
	ActorManager * actorManager;
	ResourceManager * resourceManager;
	ComponentManager* componentManager;
	ScriptSystem* scriptSystem;
	ProjectManager* projectManager;

	char *mainPath;
};

