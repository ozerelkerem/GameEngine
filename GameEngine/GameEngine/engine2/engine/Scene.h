#pragma once

#include <string>

#include<Api.h>
#include<Engine.h>

#include <engine/Prefab.h>



class Prefab;
class PrefabNode;

class Scene
{
public:

	Scene(std::string,ProjectManager*);
	~Scene();

	std::string name;

	ProjectManager *projectManager;

	ActorID rootActor;
	int actorCounter;
	
	void addActor(Prefab *, glm::vec3);
	void recursionPrefab(PrefabNode *, glm::mat4 , ActorID);
};
