#pragma once
#include "Scene.h"

Scene::Scene()
{
	rootObject = new Object("root", this);
	objectCounter = 0;
}


Scene::~Scene()
{
}
