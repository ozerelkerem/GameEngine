#pragma once

#include<engine/Model.h>
#include<engine/Material.h>
#include<vector>;

class IModelComponent
{
public:
	IModelComponent(Model *m);
	~IModelComponent();


	Model *model;

	int numberOfMaterials;
	std::vector<Material*> materials;
};

