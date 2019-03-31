#pragma once

#include <engine/systems/AnimationSystem.h>
#include <engine/physx/PhysicSystem.h>

class SystemManager
{
public:
	SystemManager(AnimationSystem *m, PhysicSystem *p);
	~SystemManager();

	AnimationSystem *m;
	PhysicSystem *p;

	void work() {
		if (m->enabled)
			m->Update();
		if (p->enabled)
			p->Update();
	};
};

