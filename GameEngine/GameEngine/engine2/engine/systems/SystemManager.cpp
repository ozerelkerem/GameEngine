#include "SystemManager.h"






SystemManager::SystemManager(AnimationSystem * m, PhysicSystem *p,ScriptSystem *s)
{
	this->m = m;
	this->p = p;
	this->s = s;
}

SystemManager::~SystemManager()
{
}
