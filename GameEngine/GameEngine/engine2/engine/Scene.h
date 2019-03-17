#pragma once

#include <string>


#include <engine/Prefab.h>
#include <engine/components/ComponentSystem.h>


class Prefab;
class PrefabNode;
class Actor;

class Scene
{
public:

	Scene(std::string,ProjectManager*);
	~Scene();

	std::string name;

	ComponentSystem *componentSystem;
	ProjectManager *projectManager;

	Actor *rootActor;
	int actorCounter;

	Actor *getActorByID(int);

	/*Find the actor by given id and root if it is exist*/
	Actor *findActor(Actor*, int);
	
	void addActor(Prefab *, glm::vec3);
	void recursionPrefab(PrefabNode *, glm::mat4 , Actor *);
};
