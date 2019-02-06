#pragma once

#include <Mesh.h>
#include <Model.h>
#include <Camera.h>
#include <Light.h>
#include <PrefabNode.h>

class Prefab
{
public:
	Prefab();
	~Prefab();

	unsigned int numberOfCameras;
	Camera **cameras;
	
	unsigned int numberOfLights;
	Light **lights;

	unsigned int numberOfModels;
	Model *(*models);

	unsigned int numberOfMaterials;
	Material *(*materials);

	PrefabNode *rootNode;

	void addModel(Model *model);
	void addLight(Light *light);
	void addCamera(Camera *camera);
	void addMaterials(Material *material);
};

