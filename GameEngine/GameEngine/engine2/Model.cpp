#include "Model.h"



Model::Model(std::string name) : Object(name)
{
	numOfMeshes = 0;
	meshes = (Mesh**)calloc(numOfMeshes + 1, sizeof(Mesh*));
}


Model::~Model()
{
}

void Model::addMesh(Mesh * m)
{
	meshes[numOfMeshes] = m;
	meshes = (Mesh**)realloc(meshes, (++numOfMeshes + 1) * sizeof(Mesh*));
}
