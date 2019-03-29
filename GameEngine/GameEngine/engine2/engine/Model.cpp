#include "Model.h"

#include<editor/Serializable.h>


Model::Model(std::string path, std::string name) : Resource(path), Namable(name)
{
	numOfMeshes = 0;
	meshes = (Mesh**)calloc(numOfMeshes + 1, sizeof(Mesh*));
}




void Model::addMesh(Mesh * m)
{
	meshes[numOfMeshes] = m;
	meshes = (Mesh**)realloc(meshes, (++numOfMeshes + 1) * sizeof(Mesh*));
}
/*
void Model::loadModelToGPU(ProjectManager *pm)
{
	if (pm != NULL)
		path = pm->path + "models\\" + name + ".model";
	for (int i = 0; i < numOfMeshes; i++)
		free(meshes[i]);
	free(meshes);
	meshes = (Mesh**)calloc(1, sizeof(Mesh*));
	Serializable::ReadModel(this);
	for (int i = 0; i < numOfMeshes; i++)
	{
		meshes[i]->loadMesh();
		meshes[i]->freeMesh();
	}
}*/

void Model::load()
{
	Serializable::ReadModel(this);
	for (int i = 0; i < numOfMeshes; i++)
	{
		meshes[i]->loadMesh();
		meshes[i]->freeMesh();
	}
}

void Model::unload()
{
	for (int i = 0; i < numOfMeshes; i++)
	{
		delete meshes[i];
	}
}
