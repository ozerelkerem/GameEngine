#include "Mesh.h"



Mesh::Mesh()
{
	//createVAO();
}

void Mesh::createVAO()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
}

void Mesh::unbindVAO()
{
	glBindVertexArray(vao);
}

void Mesh::storeData(int attribID,int num, GLenum target, GLenum type, void * buffer, int size,bool isIndices)
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

void Mesh::Render()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, numOfVertices*3, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}
