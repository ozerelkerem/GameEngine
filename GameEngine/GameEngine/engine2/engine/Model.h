#pragma once


#include <engine/Mesh.h>

class Model
{
public:
	Model(std::string);
	~Model();

	std::string name;

	unsigned int numOfMeshes;
	Mesh *(*meshes);

	void addMesh(Mesh * m);
};

