#pragma once

#include <engine/Namable.h>
#include <engine/SkinnedMesh.h>


#include<fstream>


enum ModelType { Normal, Skinned };

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

	inline ModelType getType() {
		if (dynamic_cast<SkinnedMesh *>(meshes[0]))
			return Skinned;
		else
			return Normal;
	}
};

