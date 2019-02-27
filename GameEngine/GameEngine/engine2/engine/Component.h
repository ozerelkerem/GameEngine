#pragma once
#include <string>
enum ComponentType
{
	Camera, Light, ModelComp
};

static const char *ComponentNames[] = { "Camera", "Light", "Model" };

class Component
{
public:
	Component();
	~Component();



	template <class T>
	T *copy() {
		return new T(this);
	}

	virtual ComponentType getType() = 0;
	std::string getTypeName();
	virtual Component * copy() = 0;
};

