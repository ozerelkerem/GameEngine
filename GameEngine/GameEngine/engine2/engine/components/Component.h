#pragma once
#include <string>
#include<Api.h>
#include <util/FamilyTypeID.h>
#include <engine/components/IComponent.h>

template <class T>
class Component : public IComponent
{
public:
	Component() : IComponent() {}
	~Component(){}

	static const ComponentTypeID STATIC_COMPONENT_TYPE_ID;

	inline ComponentTypeID GetStaticComponentTypeID() const
	{
		return STATIC_COMPONENT_TYPE_ID;
	}

};

template<class T>
const ComponentTypeID Component<T>::STATIC_COMPONENT_TYPE_ID = GameEngine::Util::FamilyTypeID<IComponent>::Get<T>();

