#pragma once


#include <engine/Actor.h>
#include <engine/ModelComponent.h>
#include <engine/components/LightComponent.h>

#include <unordered_map>
#include <forward_list>

class Component;
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

	 void addLightComponent(Actor *actor, Component *comp);
	 void addModelComponent(Actor *actor, Component *comp);
};


