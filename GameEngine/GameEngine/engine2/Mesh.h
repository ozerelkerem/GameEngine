#pragma once
#include <Material.h>

#include <Object.h>

class Mesh
{
public:
	Mesh(unsigned int, unsigned int, float *, float *, unsigned int *);
	~Mesh();

	Material *material;

	unsigned int numOfVertices;
	float *vertices;
	float *normals;

	unsigned int numOfIndices;
	unsigned int *indices;

	


};

