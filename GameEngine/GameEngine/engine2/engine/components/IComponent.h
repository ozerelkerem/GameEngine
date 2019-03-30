#pragma once

#include <Api.h>


using ComponentTypeID = TypeID;
using ComponentID = size_t;
class IComponent
{
public:
	static const ComponentID INVALID_COMPONENT_ID = std::numeric_limits<ComponentID>::max();

	IComponent(ActorID);
	~IComponent();

	ComponentID componentID;
	ActorID owner;

	virtual IComponent * getnew(ActorID, ComponentTypeID *) = 0;
};

