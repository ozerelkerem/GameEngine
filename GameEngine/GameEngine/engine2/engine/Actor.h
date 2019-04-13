#pragma once

#include <Api.h>

#include<engine/components/ComponentManager.h>
#include <engine/Transform.h>
#include<util/Handle.h>

class IComponent;
class Scene;
class ComponentSystem;
class ActorManager;

class Actor
{
public:
	Actor(std::string, Scene *, ActorID);
	Actor(ActorID aid) : actorID(aid) {};
	~Actor();

	ComponentManager* componentManagerInstance;

	

	ActorID actorID;

	std::string name;
	unsigned int id;
	Scene *scene;

	Transform transformation;
	ActorID parent;

	unsigned int numberOfChildren;
	ActorID *(children);

	bool AddChild(ActorID newChild);
	void RemoveActor();
	bool RemoveChild(ActorID removeChild);
	bool isParent(ActorID root);
	bool AddParent(ActorID newParent);
	bool RemoveParent();



	void processTransformation();
	void RecalculateRealMatrix();


	template<class T>
	T* GetComponent() const
	{
		return this->componentManagerInstance->GetComponent<T>(this->actorID);
	}

	template<class T, class ...P>
	T* AddComponent(P&&... param)
	{
		return this->componentManagerInstance->AddComponent<T>(this->actorID, std::forward<P>(param)...);
	}

	template<class T>
	T* AddComponent(T* other)
	{
		return this->componentManagerInstance->AddComponent<T>(this->actorID, other);
	}

	template<class T>
	void RemoveComponent()
	{
		this->componentManagerInstance->RemoveComponent<T>(this->actorID);
	}

};


