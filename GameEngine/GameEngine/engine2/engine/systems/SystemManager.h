#pragma once

#include <engine/systems/AnimationSystem.h>

class SystemManager
{
public:
	SystemManager(AnimationSystem *m);
	~SystemManager();

	AnimationSystem *m;

	void work() {
		if (m->enabled)
			m->Update();
	};
};

