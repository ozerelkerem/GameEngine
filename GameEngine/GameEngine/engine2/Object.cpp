#include "Object.h"



Object::Object(std::string name)
{
	this->name = name;
	componentObject = new ComponentObject();
}


Object::~Object()
{
}
