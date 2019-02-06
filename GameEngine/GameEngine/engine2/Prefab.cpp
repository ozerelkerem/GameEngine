#include "Prefab.h"



Prefab::Prefab()
{
	numberOfModels = 0;
	models = (Model**)calloc(numberOfModels + 1, sizeof(Model*));

	numberOfCameras = 0;
	cameras = (Camera**)calloc(numberOfCameras + 1, sizeof(Camera*));

	numberOfLights = 0;
	lights = (Light**)calloc(numberOfLights + 1, sizeof(Light*));

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

void Prefab::addLight(Light * light)
{
	lights[numberOfLights] = light;
	lights = (Light**)realloc(lights, (++numberOfLights + 1) * sizeof(Light*));
}

void Prefab::addCamera(Camera * camera)
{
	cameras[numberOfCameras] = camera;
	cameras = (Camera**)realloc(cameras, (++numberOfCameras + 1) * sizeof(Camera*));
}

void Prefab::addMaterials(Material * material)
{
	materials[numberOfMaterials] = material;
	materials = (Material**)realloc(materials, (++numberOfMaterials + 1) * sizeof(Material*));
}
