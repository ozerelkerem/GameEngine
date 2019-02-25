#include "ComponentObject.h"



ComponentObject::ComponentObject()
{
}

ComponentObject::~ComponentObject()
{
	for (auto c : componentlist)
		for (auto l : c.second)
			delete l;
}

bool ComponentObject::addComponent(Component * c)
{
	if (c->getType() < 100)
		if (!componentlist[c->getType()].empty())
			return false;

	componentlist[c->getType()].push_front(c);

	return true;
}

bool ComponentObject::hasComponent(ComponentType ct)
{
	return !componentlist[ct].empty();
}

bool ComponentObject::removeComponent(ComponentType ct)
{
	if (componentlist[ct].empty())
		return false;
	else
	{
		for (auto x : componentlist[ct])
			delete x;
		componentlist[ct].clear();

		return true;
	}
}

bool ComponentObject::removeComponent(Component * c)
{
	for (auto x : componentlist[c->getType()])
		if (x == c)
		{
			componentlist[c->getType()].remove(x);
			delete x;
			return true;
		}

	return false;
}

Component * ComponentObject::getComponentByComponentType(ComponentType ct)
{
	if (!componentlist[ct].empty())
		return componentlist[ct].front();
	else
		return NULL;
}
