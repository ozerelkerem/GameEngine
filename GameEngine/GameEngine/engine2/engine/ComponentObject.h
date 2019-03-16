#pragma once

#include <engine/components/Component.h>

#include <unordered_map>
#include <forward_list>

class ComponentObject
{
public:
	ComponentObject();
	~ComponentObject();

	std::unordered_map<ComponentTypeID, std::forward_list<IComponent *>> componentlist;
	std::unordered_map<ComponentTypeID, std::forward_list<IComponent *>> componentTypes;
	template<class T>
	inline bool addComponent(T *c);

	inline bool addComponent(ComponentTypeID,IComponent *);

	template<class T>
	inline bool hasComponent();

	template<class T>
	inline bool removeComponent();

	//bool removeComponent(Component *c);

	template<class T>
	T *getComponent();
};

template<class T>
inline bool ComponentObject::addComponent(T * c)
{
	componentlist[T::STATIC_COMPONENT_TYPE_ID].push_front(c);
	return true;
}

inline bool ComponentObject::addComponent(ComponentTypeID typid, IComponent *c)
{
	componentlist[typid].push_front(c);
	return true;
}

template<class T>
inline bool ComponentObject::hasComponent()
{
	return !componentlist[T::STATIC_COMPONENT_TYPE_ID].empty();
}

template<class T>
inline bool ComponentObject::removeComponent()
{
	if (componentlist[T::STATIC_COMPONENT_TYPE_ID].empty())
		return false;
	else
	{
		for (auto x : componentlist[T::STATIC_COMPONENT_TYPE_ID])
			delete x;
		componentlist[T::STATIC_COMPONENT_TYPE_ID].clear();

		return true;
	}
}

template<class T>
inline T * ComponentObject::getComponent()
{
	if (!componentlist[T::STATIC_COMPONENT_TYPE_ID].empty())
		return static_cast<T*>(componentlist[T::STATIC_COMPONENT_TYPE_ID].front());
	else
		return NULL;
}
