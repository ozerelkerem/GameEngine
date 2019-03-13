#pragma once




#include <unordered_map>
#include <forward_list>

class Component;
class LightComponent;
class ModelComponent;
class Model;
class Actor;


class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<Actor *>> actorsWhichContainsModelComponent;
	std::unordered_map<Actor *, LightComponent *> actorsWhichContainsLightComponent;


	void AddComponent(Actor *actor, Component *);
	void addActor(Actor *);

	void changeModel(Actor *, Model *);

	 inline void addLightComponent(Actor *actor, Component *comp);
	 inline void addModelComponent(Actor *actor, Component *comp);
};

#include <engine/Model.h>
#include <engine/Actor.h>
#include <engine/ModelComponent.h>
#include <engine/components/LightComponent.h>


inline void ComponentSystem::addLightComponent(Actor *actor, Component *comp) {
	actorsWhichContainsLightComponent[actor] = (LightComponent *)comp;
}
inline void ComponentSystem::addModelComponent(Actor *actor, Component *comp) {
	bool tf = actorsWhichContainsModelComponent[static_cast<ModelComponent *>(comp)->model].empty();
	actorsWhichContainsModelComponent[static_cast<ModelComponent *>(comp)->model].push_front(actor);
	auto model = static_cast<ModelComponent *>(comp)->model;
	if (tf);
	for (int i = 0; i < model->numOfMeshes; i++)
		model->meshes[i]->loadMesh();
}

