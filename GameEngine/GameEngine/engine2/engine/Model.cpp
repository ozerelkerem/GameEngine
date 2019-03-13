#include "Model.h"
#include<editor/Serializable.h>


Model::Model(std::string name) : Namable(name)
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

void Model::loadModelToGPU(ProjectManager *pm)
{
	std::ifstream file;
	file.open(pm->path + "\\models\\" + name + ".model");
	if (!file)
		throw std::exception("Model cannot open");
	
	Serializable::readfile(file,&numOfMeshes);
	for (int i = 0; i < numOfMeshes; i++)
	{
		Mesh *m = meshes[i];
		Serializable::readfile(file, &m->numberOfVertices);
		Serializable::readfile(file, &m->vertices, m->numberOfVertices*3);
		Serializable::readfile(file, &m->normals, m->numberOfVertices * 3);
		Serializable::readfile(file, &m->textureCoords, m->numberOfVertices * 2);
		Serializable::readfile(file, &m->numberOfIndices);
		Serializable::readfile(file, &m->indices, m->numberOfIndices * 3);
		m->loadMesh();
		m->~Mesh();
	}
}
