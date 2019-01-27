#pragma once

#include "Object.h"
#include "Mesh.h"
#include "SceneRenderer.h"

#include  <string>

class SceneRenderer;
class Transform;

class Model : public Object
{
public:
	int numOfMeshes;
	Mesh **meshes;

	Model(int numOfMeshes, Scene *scene, std::string name);

	void Render(Shader *shader);

	~Model();
};


namespace ModelFunctions
{
	static Model *CreateCube(Scene *scene)
	{
	
		Mesh *mesh = new Mesh();

		mesh->numOfVertices = 24;
		mesh->numOfIndices = 36;
		mesh->vertices = (GLfloat*)malloc(mesh->numOfVertices * 3 * sizeof(GLfloat));
		mesh->indicies = (GLuint*)malloc((mesh->numOfIndices) * sizeof(GLuint));
		mesh->numOfColors = 12;
		mesh->colors = (GLfloat*)malloc(72 * sizeof(GLfloat));

		float temp[72] = { -1,-1,-1,-1,-1,1,-1,1,1,-1,1,-1,-1,1,-1,-1,1,1,1,1,1,1,1,-1,1,1,-1,1,1,1,1,-1,1,1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,-1,-1,-1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,-1,1,1,1,-1,1,1,-1,-1,1,1,-1,1 };
		for (int i = 0; i < 72; i++)
			mesh->vertices[i] = temp[i];

		unsigned int temp2[36] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };
		for (int i = 0; i < 36; i++)
			mesh->indicies[i] = temp2[i];

		for (int i = 0; i < 72; i++)
			mesh->colors[i] = 0.9;

		mesh->createVAO();
		mesh->storeData(0, mesh->numOfIndices, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_INT, mesh->indicies, 3, true);
		mesh->storeData(0, mesh->numOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, mesh->vertices, 3);
		mesh->storeData(1, mesh->numOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, mesh->colors, 3);
		free(mesh->vertices);
		free(mesh->indicies);
		free(mesh->colors);
		mesh->unbindVAO();

		Model *model = new Model(1, scene, "Cube");
		model->meshes[0] = mesh;

		return model;
	}
}

