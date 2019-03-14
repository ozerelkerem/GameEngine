#pragma once

#include <engine/Namable.h>
#include <engine/Mesh.h>


#include<fstream>


class ProjectManager;
class ComponentSystem;

class Model : public Namable
{
public:
	Model(std::string);
	~Model();

	std::string path;

	unsigned int numOfMeshes;
	Mesh *(*meshes);

	void addMesh(Mesh * m);

	void loadModelToGPU(ProjectManager *pm=NULL);
};

