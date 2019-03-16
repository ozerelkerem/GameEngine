#pragma once

#include <Api.h>


using ComponentTypeID = TypeID;

class IComponent
{
public:

	IComponent();
	~IComponent();

	virtual IComponent * getnew(ComponentTypeID *) = 0;
};

