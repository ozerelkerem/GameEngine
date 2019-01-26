#include "Model.h"

Model::Model(int numOfMeshes, Scene *scene, std::string name) : Object(name, scene)
{
	this->numOfMeshes = numOfMeshes;
	meshes = (Mesh**)malloc(sizeof(Mesh*) * numOfMeshes);
}


Model::~Model()
{
}