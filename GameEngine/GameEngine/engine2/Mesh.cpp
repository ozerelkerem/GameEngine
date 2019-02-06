#include "Mesh.h"

Mesh::Mesh(unsigned int nov, unsigned int noi, float * vert, float * norm, unsigned int * ind)
{
	numOfVertices = nov;
	numOfIndices = noi;

	vertices = vert;
	normals = norm;
	indices = ind;
}

Mesh::~Mesh()
{
	free(vertices);
	free(normals);
	free(indices);
}
