#pragma once
#include "Scene.h"

Scene::Scene()
{
	rootObject = new Object("root", this);
}


Scene::~Scene()
{
}
