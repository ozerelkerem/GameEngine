#pragma once

#include "Object.h"

class Object;

class Scene
{
public:
	Object *rootObject;
	int objectCounter;

	Scene();

	Object * getObjectByID(int);

	Object * findObject(Object*, int);

	~Scene();
};

