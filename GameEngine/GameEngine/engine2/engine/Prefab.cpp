#include "Prefab.h"



Prefab::Prefab(std::string name)
{
	this->name = name;

	numberOfModels = 0;
	models = (Model**)calloc(numberOfModels + 1, sizeof(Model*));

	numberOfCameras = 0;
	cameras = (Object**)calloc(numberOfCameras + 1, sizeof(Object*));

	numberOfLights = 0;
	lights = (Object**)calloc(numberOfLights + 1, sizeof(Object*));

	numberOfMaterials= 0;
	materials = (Material**)calloc(numberOfMaterials + 1, sizeof(Material*));

	rootNode = NULL;
}

Prefab::~Prefab()
{

}

void Prefab::addModel(Model * model)
{
	models[numberOfModels] = model;
	models = (Model**)realloc(models, (++numberOfModels + 1) * sizeof(Model*));
}

void Prefab::addLight(Object * light)
{
	lights[numberOfLights] = light;
	lights = (Object**)realloc(lights, (++numberOfLights + 1) * sizeof(Object*));
}

void Prefab::addCamera(Object * camera)
{
	cameras[numberOfCameras] = camera;
	cameras = (Object**)realloc(cameras, (++numberOfCameras + 1) * sizeof(Object*));
}

void Prefab::addMaterials(Material * material)
{
	materials[numberOfMaterials] = material;
	materials = (Material**)realloc(materials, (++numberOfMaterials + 1) * sizeof(Material*));
}

Object *Prefab::getObject(std::string name)
{
	int i = 0;
	while (i < numberOfCameras && cameras[i++]->name != name);
	if (i-1 < numberOfCameras)
		return cameras[i - 1];

	i = 0;
	while (i < numberOfLights && lights[i++]->name != name);
	if (i -1< numberOfLights)
		return lights[i - 1];

	return NULL;

}
