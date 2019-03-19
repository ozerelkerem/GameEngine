#pragma once

#include <engine/Material.h>
#include <engine/Object.h>

#include <GL/glew.h>

#include <engine/Shader.h>

class Mesh
{
public:
	Mesh(unsigned int, unsigned int, float *, float *, unsigned int *, float * = NULL);
	~Mesh();

	unsigned int vao;

	unsigned int numberOfVertices;
	float *vertices;
	float *normals;
	float *textureCoords;

	unsigned int numberOfIndices;
	unsigned int *indices;

	struct boundstype
	{
		float minx, miny, minz;
		float maxx, maxy, maxz;
	}bounds;

	virtual void loadMesh();
	void render();


	virtual inline void bind() {
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		normalShader->setInt("isSkinned", 0);
	};
	virtual inline void unbind() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	};

	virtual inline  void freeMesh() {
		free(vertices);
		free(normals);
		free(indices);
		free(textureCoords);
	};

protected:

	void createVAO();
	void storeData(int attribID, int num, GLenum target, GLenum type, void * buffer, int size, bool isIndices = false);
	

};
