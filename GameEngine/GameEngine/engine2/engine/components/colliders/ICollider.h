#pragma once
#include <editor/ConstantModels.h>
class ICollider
{
public:
	ICollider();
	~ICollider();

	physx::PxShape *shape;

	virtual void update() = 0;
};

