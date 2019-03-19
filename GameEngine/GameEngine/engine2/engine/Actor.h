#pragma once

#include <Api.h>

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
};
#include <engine/Scene.h>


template<class T>
inline void Actor::AddComponent(IComponent *component)
{
	componentObject->addComponent<T>(reinterpret_cast<T*>(component));
	scene->componentSystem->AddComponent<T>(this->actorID, reinterpret_cast<T*>(component));
}





#include <engine/components/ComponentObject.h>