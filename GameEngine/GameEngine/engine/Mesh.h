#pragma once

#include <stdlib.h>
#include <algorithm>
#include "../ogl/GL/glew.h"


class Mesh
{
public:
	int numOfVertices;
	GLfloat *vertices;
	int numOfIndices;
	GLuint *indicies;

	int numOfColors;
	GLfloat *colors;



	Mesh();
			
	GLuint vao;
	void createVAO();
	void unbindVAO();
	void storeData(int attribID,int num, GLenum target, GLenum type, void * buffer, int size, bool isIndices=false);

	void Render();


	~Mesh();
};
