#pragma once
#include <editor/ConstantModels.h>

class ICollider
{
public:

	physx::PxShape *shape;

	virtual void update() = 0;
};

