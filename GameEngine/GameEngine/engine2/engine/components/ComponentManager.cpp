#include "ComponentManager.h"



ComponentID ComponentManager::AqcuireComponentId(IComponent * component)
{
	int i = 0;
	for (; i < this->componentLUT.size(); ++i)
	{
		if (this->componentLUT[i] == nullptr)
		{
			this->componentLUT[i] = component;
			return i;
		}
	}

	// increase component LUT size
	this->componentLUT.resize(this->componentLUT.size() + COMPONENT_LUT_GROW, nullptr);

	this->componentLUT[i] = component;
	return i;
}

void ComponentManager::ReleaseComponentId(ComponentID id)
{
	this->componentLUT[id] = nullptr;
}

void ComponentManager::MapActorComponent(ActorID actorid, ComponentID componentId, ComponentTypeID componentTypeId)
{
	static const size_t NUM_COMPONENTS{ GameEngine::Util::FamilyTypeID<IComponent>::Get() };

	if ((this->actorComponentMap.size() - 1) < actorid.index)
	{
		size_t oldSize = this->actorComponentMap.size();

		// we scale this map size along the entity lookup table size
		size_t newSize = oldSize + ACTOR_LUT_GROW;

		this->actorComponentMap.resize(newSize);

		for (auto i = oldSize; i < newSize; ++i)
			this->actorComponentMap[i].resize(NUM_COMPONENTS, IComponent::INVALID_COMPONENT_ID);
	}

	// create mapping
	this->actorComponentMap[actorid.index][componentTypeId] = componentId;
}

void ComponentManager::UnmapActorComponent(ActorID actorid, ComponentID componentId, ComponentTypeID componentTypeId)
{
	assert(this->actorComponentMap[actorid.index][componentTypeId] == componentId && "FATAL: Entity Component ID mapping corruption!");

	// free mapping
	this->actorComponentMap[actorid.index][componentTypeId] = IComponent::INVALID_COMPONENT_ID;

	// free component id
	this->ReleaseComponentId(componentId);
}

ComponentManager::ComponentManager()
{
	const size_t NUM_COMPONENTS{ GameEngine::Util::FamilyTypeID<IComponent>::Get() };

	this->actorComponentMap.resize(ACTOR_LUT_GROW);
	for (auto i = 0; i < ACTOR_LUT_GROW; ++i)
		this->actorComponentMap[i].resize(NUM_COMPONENTS, IComponent::INVALID_COMPONENT_ID);
}


ComponentManager::~ComponentManager()
{
	for (auto cc : this->componentContainerRegistry)
	{
		delete cc.second;
		cc.second = nullptr;
	}
}
