#pragma once

#include <string>

#include <Actor.h>
#include <Prefab.h>
#include <ComponentSystem.h>
class Actor;
class ComponentSystem;

class Scene
{
public:

	Scene(std::string);
	~Scene();

	std::string name;

	ComponentSystem *componentSystem;

	Actor *rootActor;
	int actorCounter;

	Actor *getActorByID(int);

	/*Find the actor by given id and root if it is exist*/
	Actor *findActor(Actor*, int);
	
	void addActor(Prefab *, glm::vec3);
	void recursionPrefab(PrefabNode *, glm::mat4 , Actor *);
};

