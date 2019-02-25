#pragma once

#include <Component.h>
#include <unordered_map>
#include <forward_list>

class ComponentObject
{
public:
	ComponentObject();
	~ComponentObject();

	std::unordered_map<ComponentType, std::forward_list<Component *>> componentlist;
	
	bool addComponent(Component *c);
	bool hasComponent(ComponentType);
	bool removeComponent(ComponentType);
	bool removeComponent(Component *c);
	Component *getComponentByComponentType(ComponentType);
};

