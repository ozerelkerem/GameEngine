#pragma once

#include <engine/Model.h>
#include <engine/components/Component.h>
#include <engine/components/IModelComponent.h>
class Model;
class ComponentSystem;

class ModelComponent : public IModelComponent, public Component<ModelComponent>
{
public:
	ModelComponent(Model *m);
	~ModelComponent();

	inline virtual IComponent * getnew(ActorID own, ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		auto x = ((IComponent*) new ModelComponent(*this));
		x->owner = own;
		return x;
	}

	inline virtual void setModel(Model *m) override {
		model = m;
	}
};

