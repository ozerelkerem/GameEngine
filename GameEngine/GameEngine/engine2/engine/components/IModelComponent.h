#pragma once

#include<engine/Model.h>
#include<engine/Material.h>
#include<vector>

class IModelComponent
{
public:
	IModelComponent();
	~IModelComponent();




	int numberOfMaterials;
	std::vector<Material*> materials;

	inline virtual void setModel(Model *m) = 0;
	inline virtual Model* getModel() { return model; };

protected:
	Model *model;
};

