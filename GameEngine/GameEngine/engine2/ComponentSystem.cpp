#include "ComponentSystem.h"



ComponentSystem::ComponentSystem()
{
}


ComponentSystem::~ComponentSystem()
{
}

void ComponentSystem::addActor(Actor *a)
{
	if (a->componentObject->hasComponent(ComponentType::ModelComp))
	{
		auto comp = a->componentObject->componentlist[ComponentType::ModelComp].front();
		actorsWhichContainsModelComponent[static_cast<ModelComponent *>(comp)->model].push_front(a);
		auto model = static_cast<ModelComponent *>(comp)->model;
		for (int i = 0; i < model->numOfMeshes; i++)
			model->meshes[i]->loadMesh();
	}
		
}
