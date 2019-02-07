#pragma once
#include "Scene.h"
#include <iostream>
Scene::Scene(std::string name)
{
	this->name = name;

	actorCounter = 1;
	rootActor = new Actor(this->name, this);
	componentSystem = new ComponentSystem();
}

Actor *Scene::getActorByID(int id)
{
	return findActor(rootActor, id);
}

Actor *Scene::findActor(Actor *root, int target)
{
	if (root->id == target)
		return root;
	for (int i = 0; i < root->numberOfChildren; i++) {
		Actor *t;
		if (t = findActor(root->children[i], target))
			return t;
	}
	return NULL;
}

void Scene::addActor(Prefab *prefab, glm::vec3 pos)
{
	recursionPrefab(prefab->rootNode, this->rootActor);
}

void Scene::recursionPrefab(PrefabNode *node, Actor *actorNode)
{
	Actor *a = new Actor(node->object->name, this);
	for (auto t : node->object->componentObject->componentlist)
		for (auto c : t.second)
			a->componentObject->addComponent(c->copy());
	
	a->transformation->relativeMatrix = node->transformation;
	a->transformation->realMatrix = node->transformation;
	a->AddParent(actorNode);
	componentSystem->addActor(a);

			
	for (int i = 0; i < node->numofChildren; i++)
	{
		recursionPrefab(node->children[i], a);
	}
}

Scene::~Scene()
{
}
