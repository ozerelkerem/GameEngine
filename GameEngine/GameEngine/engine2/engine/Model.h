#pragma once

#include <engine/Namable.h>
#include <engine/SkinnedMesh.h>
#include <engine/resource/Resource.h>

#include<fstream>


enum ModelType { Normal, Skinned };

class ProjectManager;
class ComponentSystem;

class Model : public Resource<Model> ,public Namable
{
public:
	/*
	//param1: path
	//param2: name
	*/
	Model(std::string, std::string);


	unsigned int numOfMeshes;
	Mesh *(*meshes);

	void addMesh(Mesh * m);

	//void loadModelToGPU(ProjectManager *pm=NULL);

	virtual void load() override;
	virtual void unload() override;

	inline ModelType getType() {
		if (dynamic_cast<SkinnedMesh *>(meshes[0]))
			return Skinned;
		else
			return Normal;
	}

	void operator delete(void *a) { };
};

