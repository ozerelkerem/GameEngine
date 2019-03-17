#pragma once

#include <string>

#include <engine/components/ComponentObject.h>

class Object
{
public:
	Object(std::string);
	~Object();

	
	ComponentObject *componentObject;
	std::string name;

};

