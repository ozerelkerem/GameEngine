#pragma once

#include <Api.h>


using ComponentTypeID = TypeID;
using ComponentID = size_t;
class IComponent
{
public:
	static const ComponentID INVALID_COMPONENT_ID = std::numeric_limits<ComponentID>::max();

	friend class ComponentManager;

	IComponent();
	virtual ~IComponent();

	ComponentID componentID;
	ActorID owner;


};

