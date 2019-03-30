#pragma once


#include <engine/components/ModelComponent.h>
#include <engine/components/IModelComponent.h>
class Actor;

class SkinnedModelComponent :  public Component<SkinnedModelComponent>, public IModelComponent
{
public:
	SkinnedModelComponent(Model *);
	~SkinnedModelComponent();

	ActorID rootBone;

	std::vector<std::vector<ActorID::value_type>> effectlist;


	void matchBones();


	inline virtual void setModel(Model *m) override {
		model = m;
	}
};



