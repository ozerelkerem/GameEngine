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

	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new ModelComponent(*this));
	}
};

