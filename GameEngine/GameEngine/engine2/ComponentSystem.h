#pragma once

#include <Model.h>
#include <Actor.h>
#include <ModelComponent.h>
#include <unordered_map>
#include <forward_list>

class Actor;

class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<Actor *>> actorsWhichContainsModelComponent;

	void addActor(Actor *);
};

