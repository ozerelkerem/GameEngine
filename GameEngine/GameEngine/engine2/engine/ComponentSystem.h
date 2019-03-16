#pragma once




#include <unordered_map>
#include <forward_list>

class IComponent;

class Model;
class Actor;
#include <engine/components/ModelComponent.h>
#include <engine/components/LightComponent.h>


class ComponentSystem
{
public:
	ComponentSystem();
	~ComponentSystem();

	std::unordered_map<Model *, std::forward_list<Actor *>> actorsWhichContainsModelComponent;
	std::unordered_map<Actor *, LightComponent *> actorsWhichContainsLightComponent;

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
	inline void AddComponent<LightComponent>(Actor *actor, LightComponent *component)
	{
		actorsWhichContainsLightComponent[actor] = component;
	}


	void addActor(Actor *);
	void changeModel(Actor *, Model *);

	
};

#include <engine/Model.h>






