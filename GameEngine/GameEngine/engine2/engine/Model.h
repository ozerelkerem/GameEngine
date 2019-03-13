#pragma once

#include <engine/Namable.h>
#include <engine/Mesh.h>
#include<editor/ProjectManager.h>
#include<editor/Serializable.h>
#include<fstream>

class Serializable;
class ProjectManager;
class ComponentSystem;

class Model : public Namable
{
public:
	Model(std::string);
	~Model();

	std::string name;
	std::string path;

	unsigned int numOfMeshes;
	Mesh *(*meshes);

	void addMesh(Mesh * m);

	void loadModelToGPU(ProjectManager *);
};

