#include "Actor.h"
#include <Engine.h>
#include <engine/ActorManager.h>

Actor::Actor(std::string name, Scene *scene)
{
	this->name = name;
	this->scene = scene;
	parent = actorID.INVALID_HANDLE;

	transformation = new Transform();

	numberOfChildren = 0;
	children = (ActorID*)calloc((numberOfChildren + 1), sizeof(ActorID));
	if (name != scene->name)
		AddParent(scene->rootActor);

	componentObject = new ComponentObject();
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

	delete this;
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
		GE_Engine->actorManager->GetActor(removeChild)->parent = actorID.INVALID_HANDLE;
		return true;
	}
	else
		return false;
}

bool Actor::isParent(ActorID root)
{
	Actor *node = GE_Engine->actorManager->GetActor(root);
	while (node->name != scene->name && node->id != this->id)
	{
		root = node->parent;
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

	if (parent != actorID.INVALID_HANDLE)
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
	transformation->relativeMatrix = glm::scale(glm::translate(glm::mat4(1), transformation->position) * (glm::toMat4(transformation->qRotation)), transformation->scale);
	transformation->realMatrix = GE_Engine->actorManager->GetActor(parent)->transformation->realMatrix * transformation->relativeMatrix;
}

Actor::~Actor()
{
	free(children);
}
