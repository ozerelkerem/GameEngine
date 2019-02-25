#pragma once

#include <engine/Model.h>
#include <engine/Component.h>



class ModelComponent :public Component
{
public:
	ModelComponent(Model *m);
	~ModelComponent();

	Model *model;

	ComponentType getType() { return ComponentType::ModelComp; }
	Component * copy() { return new ModelComponent(*this); }
};

