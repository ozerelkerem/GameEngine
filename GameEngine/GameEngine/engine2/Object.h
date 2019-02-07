#pragma once

#include <string>

#include <ComponentObject.h>

class Object
{
public:
	Object(std::string);
	~Object();

	
	ComponentObject *componentObject;
	std::string name;

};

