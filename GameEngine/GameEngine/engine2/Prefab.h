#pragma once


#include <Model.h>
#include <PrefabNode.h>

class Prefab
{
public:
	Prefab(std::string);
	~Prefab();

	std::string name;

	unsigned int numberOfCameras;
	Object **cameras;
	
	unsigned int numberOfLights;
	Object **lights;

	unsigned int numberOfModels;
	Model *(*models);

	unsigned int numberOfMaterials;
	Material *(*materials);

	PrefabNode *rootNode;

	void addModel(Model *model);
	void addLight(Object *light);
	void addCamera(Object *camera);
	void addMaterials(Material *material);

	Object *getObject(std::string name);
};

