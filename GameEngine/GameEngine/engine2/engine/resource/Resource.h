#pragma once
#include <Api.h>
#include <util/FamilyTypeID.h>
#include <engine/resource/IResource.h>
#include <Engine.h>
#include <engine/resource/ResourceManager.h>
template <class T>
class Resource : public IResource 
{
public:
	Resource(std::string fp) : IResource(fp) {};
	virtual ~Resource() {};

	static const ResourceTypeID STATIC_RESOURCE_TYPE_ID;

	virtual void load() {};

	void operator delete(void *a) { GE_Engine->resourceManager->removeResource<T>(static_cast<T*>(a));  };
};

template<class T>
const ResourceTypeID Resource<T>::STATIC_RESOURCE_TYPE_ID = GameEngine::Util::FamilyTypeID<IResource>::Get<T>();

