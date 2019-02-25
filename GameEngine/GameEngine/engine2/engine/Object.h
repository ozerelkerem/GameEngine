#pragma once

#include <string>

#include <engine/ComponentObject.h>

class Object
{
public:
	Object(std::string);
	~Object();

	
	ComponentObject *componentObject;
	std::string name;

};

