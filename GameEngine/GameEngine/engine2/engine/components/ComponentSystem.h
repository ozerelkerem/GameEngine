#pragma once




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
using ActorComponentMap = std::unordered_map<Actor *, T *>;

class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<Actor *>> actorsWhichContainsModelComponent;
	std::unordered_map<Actor *, LightComponent *> actorsWhichContainsLightComponent;
	std::unordered_map<Actor *, AnimatorComponent *> actorsWhichContainsAnimatorComponent;

	template<class T>
	inline ActorComponentMap<T> GetComponents()
	{

	}

	template<>
	inline ActorComponentMap<AnimatorComponent> GetComponents() { return actorsWhichContainsAnimatorComponent; };

	template<class T>
	inline void AddComponent(Actor *actor, T *component)
	{
		
	}

	template<>
	inline void AddComponent<ModelComponent>(Actor *actor, ModelComponent *component)
	{
		bool tf = actorsWhichContainsModelComponent[component->model].empty();
		actorsWhichContainsModelComponent[component->model].push_front(actor);
		/*if (tf)
			component->model->loadModelToGPU();*/

	}
	template<>
	inline void AddComponent<SkinnedModelComponent>(Actor *actor, SkinnedModelComponent *component)
	{
		bool tf = actorsWhichContainsModelComponent[component->model].empty();
		actorsWhichContainsModelComponent[component->model].push_front(actor);
		/*if (tf)
			component->model->loadModelToGPU();*/

	}
	template<>
	inline void AddComponent<LightComponent>(Actor *actor, LightComponent *component)
	{
		actorsWhichContainsLightComponent[actor] = component;
	}
	template<>
	inline void AddComponent<AnimatorComponent>(Actor *actor, AnimatorComponent *component)
	{
		actorsWhichContainsAnimatorComponent[actor] = component;
	}

	
	void addActor(Actor *);

	template<class T>
	void changeModel(Actor *, Model *);

	
};

#include <engine/Model.h>






