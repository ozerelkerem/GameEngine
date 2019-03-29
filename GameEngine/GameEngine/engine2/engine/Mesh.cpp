#include "Mesh.h"

Mesh::Mesh(unsigned int nov, unsigned int noi, float * vert, float * norm, unsigned int * ind, float *textureCoords)
{
	numberOfVertices = nov;
	numberOfIndices = noi;

	vertices = vert;
	normals = norm;
	indices = ind;
	this->textureCoords = textureCoords;
	
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vao);
}



void Mesh::loadMesh()
{
	createVAO();
	storeData(0, numberOfIndices * 3, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_INT, indices, 3, true);
	storeData(0, numberOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, vertices, 3);
	storeData(1, numberOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, normals, 3);
	if (textureCoords)
		storeData(2, numberOfVertices * 2, GL_ARRAY_BUFFER, GL_FLOAT, textureCoords, 2);

	glBindVertexArray(0);

}



void Mesh::render()
{
	glDrawElements(GL_TRIANGLES, numberOfVertices * 3, GL_UNSIGNED_INT, 0);
}

void Mesh::createVAO()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
}


