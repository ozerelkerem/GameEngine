#pragma once
#include "Scene.h"
#include<engine/Actor.h>
#include<engine/ActorManager.h>
#include<engine/components/ModelComponent.h>
#include<engine/components/SkinnedModelComponent.h>
Scene::Scene(std::string name, ProjectManager *pm) : projectManager(pm)
{
	this->name = name;

	actorCounter = 1;
	rootActor = GE_Engine->actorManager->CreateActor(this->name, this);

}


void Scene::addActor(Prefab *prefab, glm::vec3 pos)
{
	if (prefab)
		recursionPrefab(prefab->rootNode, glm::mat4(1), this->rootActor);
	else
		assert("Prefab couldn't found.");
}

void Scene::recursionPrefab(PrefabNode *node, glm::mat4 parent, ActorID actorNode)
{
	//Actor *a = new Actor(node->object->name, this);
	ActorID actorid = GE_Engine->actorManager->CreateActor(node->object->name, this);
	Actor *a = GE_Engine->actorManager->GetActor(actorid);
	for (auto t : node->object->componentObject->componentlist)
		for (auto c : t.second)
		{	
			ComponentTypeID ctid = t.first;
			if (ctid == ModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				a->AddComponent<ModelComponent>((ModelComponent*)c);
				a->GetComponent<ModelComponent>()->setModel(GE_Engine->resourceManager->getResource<Model>(((ModelComponent *)c)->getModel()->fullpath, node->object->name));
			}
			
			if (ctid == SkinnedModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				a->AddComponent<SkinnedModelComponent>((SkinnedModelComponent*)c);
				a->GetComponent<SkinnedModelComponent>()->setModel(GE_Engine->resourceManager->getResource<Model>(((SkinnedModelComponent *)c)->getModel()->fullpath, node->object->name));
			}
			
			//CHECK HERE
		/*	IComponent * ic = c->getnew(actorid,&ctid);
			a->componentObject->addComponent(ctid,ic);
			if (ctid == ModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				((ModelComponent *)ic)->model = GE_Engine->resourceManager->getResource<Model>(((ModelComponent *)c)->model->fullpath,node->object->name);
			}
			a->componentObject->addComponent(ctid, ic);
			if (ctid == SkinnedModelComponent::STATIC_COMPONENT_TYPE_ID)
			{
				((SkinnedModelComponent *)ic)->model = GE_Engine->resourceManager->getResource<Model>(((SkinnedModelComponent *)c)->model->fullpath, node->object->name);
			}*/
		}

	
	a->AddParent(actorNode);
	a->transformation.setLocalMatrix(glm::transpose(node->transformation));
	a->transformation.decomposeLocalMatrix();
	
//	componentSystem->addActor(actorid);
	for (int i = 0; i < node->numofChildren; i++)
	{
		recursionPrefab(node->children[i], glm::mat4(1), actorid);
	}
}

void Scene::calcWorldMatricies()
{
	Actor *root = GE_Engine->actorManager->GetActor(rootActor);
	for (int i = 0; i < root->numberOfChildren; i++)
		GE_Engine->actorManager->GetActor(root->children[i])->processTransformation();
}

Scene::~Scene()
{
}
