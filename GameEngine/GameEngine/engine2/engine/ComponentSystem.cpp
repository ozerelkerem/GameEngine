#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
{
}

ComponentSystem::~ComponentSystem()
{
}

void ComponentSystem::AddComponent(Actor *actor, Component *comp)
{
	if (comp->getType() == ComponentType::Light)
		addLightComponent(actor, comp);
	else if (comp->getType() == ComponentType::ModelComp)
		addModelComponent(actor, comp);
}

//@TODO ADD LIGHT COMPONENT
void ComponentSystem::addActor(Actor *a)
{
	if (a->componentObject->hasComponent(ComponentType::ModelComp))
	{
		auto comp = a->componentObject->componentlist[ComponentType::ModelComp].front();
		addModelComponent(a, comp);
	}
	if(a->componentObject->hasComponent(ComponentType::Light))
	{
		auto comp = a->componentObject->componentlist[ComponentType::Light].front();
		addLightComponent(a, comp);
	}
}

void ComponentSystem::changeModel(Actor *actor, Model *newmodel)
{
	actorsWhichContainsModelComponent[((ModelComponent *)actor->componentObject->getComponentByComponentType(ComponentType::ModelComp))->model].remove(actor);
	actorsWhichContainsModelComponent[newmodel].push_front(actor);
}



void ComponentSystem::addLightComponent(Actor *actor, Component *comp) {
	actorsWhichContainsLightComponent[actor] = (LightComponent *)comp;
}
void ComponentSystem::addModelComponent(Actor *actor, Component *comp) {
	bool tf = actorsWhichContainsModelComponent[static_cast<ModelComponent *>(comp)->model].empty();
	actorsWhichContainsModelComponent[static_cast<ModelComponent *>(comp)->model].push_front(actor);
	auto model = static_cast<ModelComponent *>(comp)->model;
	if (tf);
	for (int i = 0; i < model->numOfMeshes; i++)
		model->meshes[i]->loadMesh();
}