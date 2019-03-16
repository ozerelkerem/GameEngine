#pragma once

#include <string>

#include <engine/Transform.h>


class IComponent;
class ComponentObject;
class Scene;
class ComponentSystem;


class Actor
{
public:
	Actor(std::string, Scene *);
	~Actor();

	ComponentObject *componentObject;

	std::string name;
	unsigned int id;
	Scene *scene;

	Transform *transformation;
	Actor *parent;

	unsigned int numberOfChildren;
	Actor *(*children);

	bool AddChild(Actor * newChild);
	void RemoveActor();
	bool RemoveChild(Actor * removeChild);
	bool isParent(Actor * root);
	bool AddParent(Actor * newParent);
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
	scene->componentSystem->AddComponent<T>(this, reinterpret_cast<T*>(component));
}





#include <engine/ComponentObject.h>