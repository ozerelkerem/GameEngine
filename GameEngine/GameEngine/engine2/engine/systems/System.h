#pragma once

#include <engine/systems/ISystem.h>
#include <util/FamilyTypeID.h>

template<class T>
class System : public ISystem
{
public:
	System(){}
	~System(){}

	static const SystemTypeID STATIC_SYSTEM_TYPE_ID;

	inline SystemTypeID GetStaticSystemTypeID() const
	{
		return STATIC_SYSTEM_TYPE_ID;
	}

	virtual void PreUpdate() override {}
	virtual void Update() override {}
	virtual void PostUpdate() override {}

};

template<class T>
const SystemTypeID System<T>::STATIC_SYSTEM_TYPE_ID = GameEngine::Util::FamilyTypeID<ISystem>::Get<T>();




