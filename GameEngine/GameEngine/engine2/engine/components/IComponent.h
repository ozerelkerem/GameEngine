#pragma once

#include <Api.h>


using ComponentTypeID = TypeID;

class IComponent
{
public:

	IComponent(ActorID);
	~IComponent();

	ActorID owner;

	virtual IComponent * getnew(ActorID, ComponentTypeID *) = 0;
};

