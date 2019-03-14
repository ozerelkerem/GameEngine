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

	free(vertices);
	free(normals);
	free(indices);
	if (textureCoords)
		free(textureCoords);
}

void Mesh::freeMesh()
{
	free(vertices);
	free(normals);
	free(indices);
	if (textureCoords)
		free(textureCoords);
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

void Mesh::bind()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Mesh::unbind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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

void Mesh::storeData(int attribID, int num, GLenum target, GLenum type, void * buffer, int size, bool isIndices)
{
	GLuint vboid;
	glGenBuffers(1, &vboid);
	glBindBuffer(target, vboid);
	glBufferData(target, sizeof(buffer) * num, buffer, GL_STATIC_DRAW);
	/*
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/
	if (!isIndices)
	{
		glVertexAttribPointer(attribID, size, type, false, 0, 0);
		glBindBuffer(target, 0);
	}


}
