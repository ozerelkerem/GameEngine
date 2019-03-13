#pragma once

#include <engine/Model.h>
#include <engine/Component.h>

class Model;
class Component;
class ComponentSystem;

class ModelComponent :public Component
{
public:
	ModelComponent(Model *m);
	~ModelComponent();

	Model *model;

	int numberOfMaterials;
	std::vector<Material*> materials;

	ComponentType getType() { return ComponentType::ModelComp; }
	Component * copy() { return new ModelComponent(*this); }
};

