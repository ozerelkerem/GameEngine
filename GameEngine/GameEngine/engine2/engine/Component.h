#pragma once

enum ComponentType
{
	Camera, Light, ModelComp
};

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
	virtual Component * copy() = 0;
};

