#pragma once

#include <engine/Model.h>
#include <engine/components/Component.h>

class Model;
class ComponentSystem;

class ModelComponent : public Component<ModelComponent>
{
public:
	ModelComponent(Model *m);
	~ModelComponent();

	Model *model;

	int numberOfMaterials;
	std::vector<Material*> materials;

	
	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new ModelComponent(*this));
	}
};

