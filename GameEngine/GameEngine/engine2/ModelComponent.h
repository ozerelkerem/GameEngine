#pragma once

#include <Model.h>
#include <Component.h>



class ModelComponent :public Component
{
public:
	ModelComponent(Model *m);
	~ModelComponent();

	Model *model;

	ComponentType getType() { return ComponentType::ModelComp; }
	Component * copy() { return new ModelComponent(*this); }
};

