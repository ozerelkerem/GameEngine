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



	inline virtual void setModel(Model *m) override {
		model = m;
	}
};

