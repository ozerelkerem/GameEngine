#include "Model.h"

Model::Model(int numOfMeshes, Scene *scene, std::string name) : Object(name, scene)
{
	this->numOfMeshes = numOfMeshes;
	meshes = (Mesh**)malloc(sizeof(Mesh*) * numOfMeshes);
}

void Model::Render(Shader *shader)
{
	shader->setMat4("modelMatrix", transform->modelMatrix);
	for (int i = 0; i < numOfMeshes; i++)
		meshes[i]->Render();
}


Model::~Model()
{
}