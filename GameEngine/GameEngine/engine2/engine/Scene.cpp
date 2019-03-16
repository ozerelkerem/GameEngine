#pragma once
#include "Scene.h"
#include <iostream>
#include<engine/Actor.h>

Scene::Scene(std::string name, ProjectManager *pm) : projectManager(pm)
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
	recursionPrefab(prefab->rootNode, glm::mat4(1), this->rootActor);
}

void Scene::recursionPrefab(PrefabNode *node, glm::mat4 parent, Actor *actorNode)
{
	Actor *a = new Actor(node->object->name, this);
	for (auto t : node->object->componentObject->componentlist)
		for (auto c : t.second)
		{	
			ComponentTypeID ctid;
			
			IComponent * ic = c->getnew(&ctid);
			a->componentObject->addComponent(ctid,ic);
			if (ctid == ModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				((ModelComponent *)c)->model->loadModelToGPU(projectManager);
			}
		}
			
	
	a->transformation->relativeMatrix = glm::transpose(node->transformation);
	glm::mat4 temp = parent *a->transformation->relativeMatrix ;
	a->transformation->realMatrix = temp;
	a->transformation->divideRealMatrix();
	a->AddParent(actorNode);
	componentSystem->addActor(a);
			
	for (int i = 0; i < node->numofChildren; i++)
	{
		recursionPrefab(node->children[i], temp, a);
	}
}

Scene::~Scene()
{
}
