#pragma once

#include <engine/Material.h>
#include <engine/Object.h>

#include <GL/glew.h>

class Mesh
{
public:
	Mesh(unsigned int, unsigned int, float *, float *, unsigned int *);
	~Mesh();

	unsigned int vao;

	unsigned int numberOfVertices;
	float *vertices;
	float *normals;

	unsigned int numberOfIndices;
	unsigned int *indices;

	

	void loadMesh();
	void bind();
	void unbind();
	void render();

private:


	void createVAO();
	void storeData(int attribID, int num, GLenum target, GLenum type, void * buffer, int size, bool isIndices = false);
	



};

