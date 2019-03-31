#include "SystemManager.h"






SystemManager::SystemManager(AnimationSystem * m, PhysicSystem *p)
{
	this->m = m;
	this->p = p;
}

SystemManager::~SystemManager()
{
}
