#pragma once


#include <engine/Model.h>
#include <engine/PrefabNode.h>

class Model;

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

