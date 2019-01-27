#pragma once
#include "Scene.h"

Scene::Scene()
{
	objectCounter = 1;
	rootObject = new Object("root", this);
	
}

Object * Scene::getObjectByID(int id)
{
	return findObject(rootObject, id);
}

Object * Scene::findObject(Object *root, int target)
{
	if (root->id == target)
		return root;
	for (int i = 0; i < root->numOfChilds; i++) {
		Object *t;
		if (t = findObject(root->childs[i], target))
			return t;
	}
	return NULL;
}


Scene::~Scene()
{
}
