#pragma once
#include "Scene.h"
#include<engine/Actor.h>
#include<engine/ActorManager.h>
Scene::Scene(std::string name, ProjectManager *pm) : projectManager(pm)
{
	this->name = name;

	actorCounter = 1;
	rootActor = GE_Engine->actorManager->CreateActor(this->name, this);
	componentSystem = new ComponentSystem();
}


void Scene::addActor(Prefab *prefab, glm::vec3 pos)
{
	recursionPrefab(prefab->rootNode, glm::mat4(1), this->rootActor);
}

void Scene::recursionPrefab(PrefabNode *node, glm::mat4 parent, ActorID actorNode)
{
	//Actor *a = new Actor(node->object->name, this);
	ActorID actorid = GE_Engine->actorManager->CreateActor(node->object->name, this);
	Actor *a = GE_Engine->actorManager->GetActor(actorid);
	for (auto t : node->object->componentObject->componentlist)
		for (auto c : t.second)
		{	
			ComponentTypeID ctid;
			
			IComponent * ic = c->getnew(actorid,&ctid);
			a->componentObject->addComponent(ctid,ic);
			if (ctid == ModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				((ModelComponent *)c)->model->loadModelToGPU(projectManager);
			}
			a->componentObject->addComponent(ctid, ic);
			if (ctid == SkinnedModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				((SkinnedModelComponent *)c)->model->loadModelToGPU(projectManager);
			}
		}

	
	
	a->transformation->relativeMatrix = glm::transpose(node->transformation);
	glm::mat4 temp = parent *a->transformation->relativeMatrix ;
	a->transformation->realMatrix = temp;
	a->transformation->divideRealMatrix();
	a->AddParent(actorNode);
	componentSystem->addActor(actorid);
	for (int i = 0; i < node->numofChildren; i++)
	{
		recursionPrefab(node->children[i], temp, actorid);
	}
}

Scene::~Scene()
{
}
