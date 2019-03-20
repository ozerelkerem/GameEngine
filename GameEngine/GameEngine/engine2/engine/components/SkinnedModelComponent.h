#pragma once

#include <engine/components/ModelComponent.h>
#include <engine/components/IModelComponent.h>
class Actor;

class SkinnedModelComponent : public IModelComponent, public Component<SkinnedModelComponent>
{
public:
	SkinnedModelComponent(ActorID, Model *);
	~SkinnedModelComponent();

	Actor *rootBone;

	inline virtual IComponent * getnew(ActorID own, ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		auto x = ((IComponent*) new SkinnedModelComponent(*this));
		x->owner = own;
		return x;
	}
};



