#pragma once

#include "Object.h"

class Object;

class Scene
{
public:
	Object *rootObject;
	int objectCounter;

	Scene();
	~Scene();
};

