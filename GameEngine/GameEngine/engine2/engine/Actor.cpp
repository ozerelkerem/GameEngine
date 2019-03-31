#include "Actor.h"
#include <Engine.h>
#include <engine/ActorManager.h>
#include <engine/Scene.h>
Actor::Actor(std::string name, Scene *scene, ActorID aid) : actorID(aid)
{
	this->name = name;
	this->scene = scene;
	parent = ActorID::INVALID_HANDLE;

	transformation = new Transform();

	numberOfChildren = 0;
	children = (ActorID*)calloc((numberOfChildren + 1), sizeof(ActorID));

	if (name != scene->name)
		AddParent(scene->rootActor);
}




bool Actor::AddChild(ActorID newChild)
{
	children[numberOfChildren] = newChild;

	numberOfChildren++;

	children = (ActorID*)realloc(children, sizeof(ActorID) * (numberOfChildren + 1));
	if (children)
		return true;
	else
		return false;
}

void Actor::RemoveActor()
{
	
	GE_Engine->actorManager->GetActor(parent)->RemoveChild(this->actorID);

	for (int i = 0; i < numberOfChildren; i++)
		GE_Engine->actorManager->GetActor(children[i])->RemoveParent();

//	scene->componentSystem->removeActor(this->actorID);
	GE_Engine->actorManager->DestroyActor(this->actorID);


	//TODO REMOVE ACTOR FROM EVERYWHERE ActorManager, ComponenentManager vs..
}

bool Actor::RemoveChild(ActorID removeChild)
{
	int i = 0;
	while (i < numberOfChildren && children[i] != removeChild) {
		i++;
	}
	if (i < numberOfChildren)
	{
		children[i] = children[--numberOfChildren];
		children = (ActorID*)realloc(children, sizeof(ActorID ) * (numberOfChildren + 1));
		GE_Engine->actorManager->GetActor(removeChild)->parent = ActorID::INVALID_HANDLE;
		return true;
	}
	else
		return false;
}

bool Actor::isParent(ActorID root)
{
	Actor *node = GE_Engine->actorManager->GetActor(root);
	if (!node)
		return false;
	while (node->name != scene->name && node->actorID != this->actorID)
	{
		node = GE_Engine->actorManager->GetActor(node->parent);
	}
	if (node->name == scene->name)
		return false;
	else
		return true;
}

bool Actor::AddParent(ActorID newParent)
{
	if (this->isParent(newParent))
	{
		GE_Engine->actorManager->GetActor(newParent)->AddParent(this->parent);
	}

	if (parent != ActorID::INVALID_HANDLE)
	{
		GE_Engine->actorManager->GetActor(parent)->RemoveChild(this->actorID);
	}

	parent = newParent;
	GE_Engine->actorManager->GetActor(parent)->AddChild(this->actorID);

	return true;
}

bool Actor::RemoveParent()
{
	GE_Engine->actorManager->GetActor(parent)->RemoveChild(this->actorID);
	this->AddParent(scene->rootActor);

	return true;
}




void Actor::processTransformation()
{
	RecalculateRealMatrix();
	
	for (int i = 0; i < numberOfChildren; i++)
	{
		GE_Engine->actorManager->GetActor(children[i])->processTransformation();
	}
}

void Actor::RecalculateRealMatrix()
{
	transformation->calcLocalMatrix();
	transformation->worldMatrix = GE_Engine->actorManager->GetActor(parent)->transformation->worldMatrix * transformation->localMatrix;
}

Actor::~Actor()
{
	free(children);
}
