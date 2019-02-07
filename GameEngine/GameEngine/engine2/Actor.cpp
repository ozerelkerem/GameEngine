#include "Actor.h"

Actor::Actor(std::string name, Scene *scene)
{
	this->name = name;
	this->scene = scene;
	id = scene->actorCounter++;
	transformation = new Transform();
	numberOfChildren = 0;
	children = (Actor**)calloc((numberOfChildren + 1), sizeof(Actor*));
	if (name != scene->name)
		AddParent(scene->rootActor);

	componentObject = new ComponentObject();
}

bool Actor::AddChild(Actor *newChild)
{
	children[numberOfChildren] = newChild;

	numberOfChildren++;

	children = (Actor**)realloc(children, sizeof(Actor*) * (numberOfChildren + 1));
	if (children)
		return true;
	else
		return false;
}

void Actor::RemoveActor()
{
	parent->RemoveChild(this);

	for (int i = 0; i < numberOfChildren; i++)
		children[i]->RemoveParent();

	delete this;
}

bool Actor::RemoveChild(Actor * removeChild)
{
	int i = 0;
	while (i < numberOfChildren && children[i]->id != removeChild->id) {
		i++;
	}
	if (i < numberOfChildren)
	{
		children[i] = children[--numberOfChildren];
		children = (Actor**)realloc(children, sizeof(Actor *) * (numberOfChildren + 1));
		removeChild->parent = NULL;
		return true;
	}
	else
		return false;
}

bool Actor::isParent(Actor *root)
{
	Actor *node = root;
	while (root->name != scene->name && root->id != this->id)
	{
		root = root->parent;
	}
	if (root->name == scene->name)
		return false;
	else
		return true;
}

bool Actor::AddParent(Actor * newParent)
{
	if (this->isParent(newParent))
	{
		newParent->AddParent(this->parent);
	}

	if (parent != NULL)
	{
		parent->RemoveChild(this);
	}

	parent = newParent;
	parent->AddChild(this);

	return true;
}

bool Actor::RemoveParent()
{
	parent->RemoveChild(this);
	this->AddParent(scene->rootActor);

	return true;
}

Actor::~Actor()
{
	free(children);
}
