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

void Mesh::storeData(int attribID,int num, GLenum target, GLenum type, void * buffer, bool isIndices)
{
	GLuint vboid;
	glGenBuffers(1, &vboid);
	glBindBuffer(target, vboid);
	glBufferData(target, num, buffer, GL_STATIC_DRAW);
	if(!isIndices)
		glVertexAttribPointer(attribID, 3, type, false, 0, 0);
	glBindBuffer(target, 0);
}

void Mesh::Render()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	//glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}
