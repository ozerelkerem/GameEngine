#pragma once
#include "Object.h"

Object::Object(std::string name, Scene *scene)
{
	this->name = name;
	this->scene = scene;
	id = scene->objectCounter++;
	transform = new Transform();
	numOfChilds = 0;
	childs = (Object**)calloc((numOfChilds + 1), sizeof(Object*));
	if(name != "root")
		AddParent(scene->rootObject);
}

bool Object::AddChild(Object *newChild)
{
	childs[numOfChilds] = newChild;

	numOfChilds++;

	childs = (Object**)realloc(childs, sizeof(Object*) * (numOfChilds + 1));
	if (childs)
		return true;
	else
		return false;
}

void Object::RemoveObject()
{
	parent->RemoveChild(this);

	for (int i = 0; i < numOfChilds; i++)
		childs[i]->RemoveParent();

	delete this;
}

bool Object::RemoveChild(Object * removeChild)
{
	int i = 0;
 	while (i < numOfChilds && childs[i]->name != removeChild->name) {
		i++;
	}
	if (i < numOfChilds)
	{
		childs[i] = childs[--numOfChilds];
		childs = (Object**)realloc(childs, sizeof(Object *) * (numOfChilds + 1));
		removeChild->parent = NULL;
		return true;
	}
	else
		return false;
}

bool Object::isParent(Object *root)
{
	Object *node = root;
	while (root->name != "root" && root->name != this->name)
	{
		root = root->parent;
	}
	if (root->name == "root")
		return false;
	else
		return true;
}

bool Object::AddParent(Object * newParent)
{
	if (this->isParent(newParent))
	{
		newParent->AddParent(this->parent);
	}

	if (parent != NULL)
	{
		parent->RemoveChild(this);
	}

	parent = newParent;
	parent->AddChild(this);

	return true;
}

bool Object::RemoveParent()
{
	parent->RemoveChild(this);
	this->AddParent(scene->rootObject);

	return true;
}

Object::~Object()
{
	free(childs);
}
