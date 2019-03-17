#pragma once

#include<Api.h>

using SystemTypeID = TypeID;

class ISystem
{
public:
	ISystem();
	~ISystem();

	uint8_t enabled : 1;

	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;

};

