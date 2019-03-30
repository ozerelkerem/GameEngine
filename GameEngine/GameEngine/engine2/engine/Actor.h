#pragma once

#include <Api.h>

#include<engine/components/ComponentManager.h>
#include <engine/Transform.h>
#include<util/Handle.h>

class IComponent;
class ComponentObject;
class Scene;
class ComponentSystem;


class Actor
{
public:
	Actor(std::string, Scene *, ActorID);
	~Actor();

	ComponentManager* componentManagerInstance;

	ComponentObject *componentObject;

	ActorID actorID;

	std::string name;
	unsigned int id;
	Scene *scene;

	Transform *transformation;
	ActorID parent;

	unsigned int numberOfChildren;
	ActorID *(children);

	bool AddChild(ActorID newChild);
	void RemoveActor();
	bool RemoveChild(ActorID removeChild);
	bool isParent(ActorID root);
	bool AddParent(ActorID newParent);
	bool RemoveParent();

	template<class T>
	inline void AddComponent(IComponent *component);

	void processTransformation();
	void RecalculateRealMatrix();


	template<class T>
	T* GetComponent() const
	{
		return this->m_ComponentManagerInstance->GetComponent<T>(this->m_EntityID);
	}

	template<class T, class ...P>
	T* AddComponent(P&&... param)
	{
		return this->m_ComponentManagerInstance->AddComponent<T>(this->m_EntityID, std::forward<P>(param)...);
	}

	template<class T>
	void RemoveComponent()
	{
		this->m_ComponentManagerInstance->RemoveComponent<T>(this->m_EntityID);
	}

};
#include <engine/Scene.h>


template<class T>
inline void Actor::AddComponent(IComponent *component)
{
	componentObject->addComponent<T>(reinterpret_cast<T*>(component));
	scene->componentSystem->AddComponent<T>(this->actorID, reinterpret_cast<T*>(component));
}





#include <engine/components/ComponentObject.h>