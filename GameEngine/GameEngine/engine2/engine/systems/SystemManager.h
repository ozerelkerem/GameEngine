#pragma once

#include <engine/systems/AnimationSystem.h>
#include <engine/physx/PhysicSystem.h>
#include <engine/scripting/ScriptSystem.h>
class SystemManager
{
public:
	SystemManager(AnimationSystem *m, PhysicSystem *p, ScriptSystem *s);
	~SystemManager();

	AnimationSystem *m;
	PhysicSystem *p;
	ScriptSystem *s;


	void work() {
		if (m->enabled)
			m->Update();
		if (p->enabled)
			p->Update();
		if (s->enabled)
			s->Update();
	};
};

