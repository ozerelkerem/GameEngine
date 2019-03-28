#pragma once



#include<Api.h>
#include <unordered_map>
#include <forward_list>

class IComponent;

class Model;
class Actor;
#include <engine/components/ModelComponent.h>
#include <engine/components/LightComponent.h>
#include<engine/components/SkinnedModelComponent.h>
#include<engine/components/AnimatorComponent.h>

template<class T>
using ActorComponentMap = std::unordered_map<ActorID::value_type , T *>;

class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<ActorID::value_type>> actorsWhichContainsModelComponent;
	std::unordered_map<Model *, std::forward_list<ActorID::value_type>> actorsWhichContainsSkinnedModelComponent;
	ActorComponentMap<LightComponent> actorsWhichContainsLightComponent;
	ActorComponentMap<AnimatorComponent> actorsWhichContainsAnimatorComponent;

	template<class T>
	inline ActorComponentMap<T> GetComponents()
	{

	}

	template<>
	inline ActorComponentMap<AnimatorComponent> GetComponents() { return actorsWhichContainsAnimatorComponent; };

	template<class T>
	inline void AddComponent(ActorID actor, T *component)
	{
		
	}

	template<>
	inline void AddComponent<ModelComponent>(ActorID actor, ModelComponent *component)
	{
		bool tf = actorsWhichContainsModelComponent[component->model].empty();
		actorsWhichContainsModelComponent[component->model].push_front(actor);
		/*if (tf)
			component->model->loadModelToGPU();*/

	}
	template<>
	inline void AddComponent<SkinnedModelComponent>(ActorID actor, SkinnedModelComponent *component)
	{
		bool tf = actorsWhichContainsSkinnedModelComponent[component->model].empty();
		actorsWhichContainsSkinnedModelComponent[component->model].push_front(actor);
		/*if (tf)
			component->model->loadModelToGPU();*/

	}
	template<>
	inline void AddComponent<LightComponent>(ActorID actor, LightComponent *component)
	{
		actorsWhichContainsLightComponent[actor] = component;
	}
	template<>
	inline void AddComponent<AnimatorComponent>(ActorID actor, AnimatorComponent *component)
	{
		actorsWhichContainsAnimatorComponent[actor] = component;
	}

	
	void addActor(ActorID);

	template<class T>
	void changeModel(ActorID, Model *);

	void removeActor(ActorID);


	
};

#include <engine/Model.h>






