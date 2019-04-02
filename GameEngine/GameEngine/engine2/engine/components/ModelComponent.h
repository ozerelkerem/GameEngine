#pragma once

#include <engine/Model.h>
#include <engine/components/Component.h>
#include <engine/components/IModelComponent.h>
class Model;
class ComponentSystem;

class ModelComponent :public Component<ModelComponent> , public IModelComponent
{
public:
	ModelComponent(Model *m);
	~ModelComponent();



	inline virtual void setModel(Model *m) override {
		model = m;
	}
};

