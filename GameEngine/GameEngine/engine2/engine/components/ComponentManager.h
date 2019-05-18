#pragma once
#include<Api.h>
#include<memory/MemoryChuckAllocator.h>

#include<engine/components/colliders/CapsuleColliderComponent.h>
#include<engine/components/colliders/CubeColliderComponent.h>
#include<engine/components/colliders/CapsuleColliderComponent.h>
#include<engine/components/colliders/SphereColliderComponent.h>
#include<engine/components/RigidBodyComponent.h>




class ComponentManager : GlobalMemoryUser
{
	friend class IComponent;


	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() {}

		virtual const char* GetComponentContainerTypeName() const = 0;
		virtual void DestroyComponent(IComponent *object) = 0;
	};

	template<class T>
	class ComponentContainer : public MemoryChunkAllocator<T, COMPONENT_CHUNK_SIZE>, public IComponentContainer
	{
		ComponentContainer(const ComponentContainer&) = delete;
		ComponentContainer& operator=(ComponentContainer&) = delete;

	public:

		ComponentContainer() : MemoryChunkAllocator<T, COMPONENT_CHUNK_SIZE>()
		{}

		virtual ~ComponentContainer() {}

		virtual const char* GetComponentContainerTypeName() const override {
			static const char* COMPONENT_TYPE_NAME{ typeid(T).name() };
			return COMPONENT_TYPE_NAME;
		}

		virtual void DestroyComponent(IComponent *object) override {
			// call d'tor
			object->~IComponent();

			this->DestroyObject(object);
		}


	};

	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator=(ComponentManager&) = delete;

	using ComponentContainerRegistry = std::unordered_map<ComponentTypeID, IComponentContainer*>;
	ComponentContainerRegistry componentContainerRegistry;

	template<class T>
	inline ComponentContainer<T>* GetComponentContainer()
	{
		ComponentTypeID CID = T::STATIC_COMPONENT_TYPE_ID;
		auto it = componentContainerRegistry.find(CID);
		
		ComponentContainer<T> *cc = nullptr;

		if (it == componentContainerRegistry.end())
		{
			cc = new ComponentContainer<T>();
			componentContainerRegistry[CID] = cc;
		}
		else
			cc = static_cast<ComponentContainer<T>*>(it->second);

		assert(cc != nullptr && "Failed to get component container");


		return cc;
	}

	using ComponentLookupTable = std::vector<IComponent*>;
	ComponentLookupTable	componentLUT;

	using ActorComponentMap = std::vector<std::vector<ComponentID>>;
	ActorComponentMap		actorComponentMap;

	ComponentID	AqcuireComponentId(IComponent* component);
	void		ReleaseComponentId(ComponentID id);

	void		MapActorComponent(ActorID actorid, ComponentID componentId, ComponentTypeID componentTypeId);
	void		UnmapActorComponent(ActorID actorid, ComponentID componentId, ComponentTypeID componentTypeId);


public:

	template<class T>
	using TComponentIterator = typename ComponentContainer<T>::iterator;

	

	ComponentManager();
	~ComponentManager();


	template<class T, class ...ARGS>
	T* AddComponent(const ActorID actorid, ARGS&&... args);
	
	template<class T>
	T* AddComponent(const ActorID actorid, T* other)
	{

		//	static constexpr std::hash<ComponentID> ACTOR_COMPONENT_ID_HASHER{ std::hash<ComponentID>() };

		const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

		void *pObjectMemory = GetComponentContainer<T>()->CreateObject();

		ComponentID componentid = this->AqcuireComponentId(static_cast<IComponent*>(pObjectMemory));


		memcpy(pObjectMemory, other, sizeof(T));
		((T*)pObjectMemory)->componentID = componentid;
		((T*)pObjectMemory)->owner = actorid;

		MapActorComponent(actorid, componentid, CTID);

		return static_cast<T*>(pObjectMemory);

	}

	template<class T>
	void RemoveComponent(const ActorID actorid)
	{
		const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

		const ComponentID componentId = this->actorComponentMap[actorid.index][CTID];

		IComponent* component = this->componentLUT[componentId];

		if constexpr (std::is_same<T, RigidBodyComponent>::value || std::is_same<T, CubeColliderComponent>::value || std::is_same<T, SphereColliderComponent>::value || std::is_same<T, CapsuleColliderComponent>::value)
		{
			GE_Engine->physicSystem->removeComponent<T>(static_cast<T*>(component));
		}

		assert(component != nullptr && "FATAL: Trying to remove a component which is not used by this entity!");

		GetComponentContainer<T>()->DestroyObject(component);
		
		UnmapActorComponent(actorid, componentId, CTID);
	}

	void RemoveAllComponents(const ActorID actorid)
	{
		static const size_t NUM_COMPONENTS = this->actorComponentMap[0].size();
		for (ComponentTypeID componentTypeId = 0; componentTypeId < NUM_COMPONENTS; ++componentTypeId)
		{
			const ComponentID componentId = this->actorComponentMap[actorid.index][componentTypeId];
			if (componentId == IComponent::INVALID_COMPONENT_ID)
				continue;

			IComponent* component = this->componentLUT[componentId];
			if (component != nullptr)
			{
				// get appropriate component container
				auto it = this->componentContainerRegistry.find(componentTypeId);
				if (it != this->componentContainerRegistry.end())
					it->second->DestroyComponent(component);
				else
					assert(false && "Trying to release a component that wasn't created by ComponentManager!");

				// unmap entity id to component id
				UnmapActorComponent(actorid, componentId, componentTypeId);
			}
		}
	}

	template<class T>
	T* GetComponent(const ActorID actorid)
	{
		const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;
		if (actorid.index >= this->actorComponentMap.size())
			return nullptr;
		const ComponentID componentId = this->actorComponentMap[actorid.index][CTID];

		// entity has no component of type T
		if (componentId == IComponent::INVALID_COMPONENT_ID)
			return nullptr;

		return reinterpret_cast<T*>(this->componentLUT[componentId]);
	}

	template<class T>
	inline TComponentIterator<T> begin()
	{
		return GetComponentContainer<T>()->begin();
	}

	template<class T>
	inline TComponentIterator<T> end()
	{
		return GetComponentContainer<T>()->end();
	}
};

#include<engine/physx/PhysicSystem.h>

template<class T, class ...ARGS>
inline T * ComponentManager::AddComponent(const ActorID actorid, ARGS && ...args)
{
const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

	void *pObjectMemory = GetComponentContainer<T>()->CreateObject();

	ComponentID componentid = this->AqcuireComponentId(static_cast<IComponent*>(pObjectMemory));
	((T*)pObjectMemory)->componentID = componentid;

	IComponent* component = new (pObjectMemory)T(std::forward<ARGS>(args)...);
	component->owner = actorid;

	MapActorComponent(actorid, componentid, CTID);

	if constexpr (std::is_same<T, RigidBodyComponent>::value || std::is_same<T, CubeColliderComponent>::value || std::is_same<T, SphereColliderComponent>::value || std::is_same<T, CapsuleColliderComponent>::value)
	{
		GE_Engine->physicSystem->addComponent<T>(static_cast<T*>(component));
	}
	return (T*)(component);
}
