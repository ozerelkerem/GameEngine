#pragma once

#include <engine/Model.h>
#include <engine/Actor.h>
#include <engine/ModelComponent.h>
#include <engine/components/LightComponent.h>

#include <unordered_map>
#include <forward_list>

class Actor;

struct LightHolder
{
	Actor * actor;
	LightComponent * lightcomp;
};

class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<Actor *>> actorsWhichContainsModelComponent;
	std::list<LightHolder *> actorsWhichContainsLightComponent;

	void addActor(Actor *);
};

