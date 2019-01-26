#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Scene.h"

#include  <string>

class Model : public Object
{
public:
	int numOfMeshes;
	Mesh **meshes;

	Model(int numOfMeshes, Scene *scene, std::string name);
	~Model();
};

namespace ModelFunctions
{
	static Model *CreateCube(Scene *scene)
	{
	
		Mesh *mesh = new Mesh();

		mesh->numOfVertices = 12;
		mesh->numOfIndices = 18;
		mesh->vertices = (GLfloat*)malloc(9 * sizeof(GLfloat));
		mesh->indicies = (GLuint*)malloc((36) * sizeof(GLuint));
		mesh->numOfColors = 12;
		mesh->colors = (GLfloat*)malloc(72 * sizeof(GLfloat));

		float temp[36] = { -1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,1,1,-1,1,-1,-1,1,-1,-1,1,1,-1,1,1,1,1,-1,1 };
		for (int i = 0; i < 36; i++)
			mesh->vertices[i] = temp[i];

		unsigned int temp2[18] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11 };
		for (int i = 0; i < 18; i++)
			mesh->indicies[i] = temp2[i];

		for (int i = 0; i < 36; i++)
			mesh->colors[i] = 1.0f;

		mesh->createVAO();
		mesh->storeData(0, mesh->numOfVertices*3, GL_ARRAY_BUFFER, GL_FLOAT, mesh->vertices, 3);
		mesh->storeData(0, mesh->numOfIndices, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_INT, mesh->indicies, 3, true);
		mesh->unbindVAO();

		Model *model = new Model(1, scene, "Cube");
		model->meshes[0] = mesh;

		return model;
	}
}

