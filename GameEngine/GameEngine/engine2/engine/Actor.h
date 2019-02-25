#pragma once

#include <string>

#include <engine/Transform.h>
#include <engine/Scene.h>
#include <engine/ComponentObject.h>

class Scene;

class Actor
{
public:
	Actor(std::string, Scene *);
	~Actor();

	ComponentObject *componentObject;

	std::string name;
	unsigned int id;
	Scene *scene;

	Transform *transformation;
	Actor *parent;

	unsigned int numberOfChildren;
	Actor *(*children);

	bool AddChild(Actor * newChild);
	void RemoveActor();
	bool RemoveChild(Actor * removeChild);
	bool isParent(Actor * root);
	bool AddParent(Actor * newParent);
	bool RemoveParent();

	void processTransformation();
	void RecalculateRealMatrix();
};

