#pragma once

#include <Object.h>
#include <Mesh.h>

class Model : public Object
{
public:
	Model(std::string);
	~Model();

	unsigned int numOfMeshes;
	Mesh *(*meshes);

	void addMesh(Mesh * m);


};

