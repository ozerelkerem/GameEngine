#pragma once

#include "Object.h"

class Object;

class Scene
{
public:
	Object *rootObject;

	Scene();
	~Scene();
};

