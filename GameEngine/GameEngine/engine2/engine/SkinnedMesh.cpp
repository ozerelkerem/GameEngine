#include "SkinnedMesh.h"






SkinnedMesh::SkinnedMesh(unsigned int nov, unsigned int noi, float * vert, float * norm, unsigned int * ind, float *textureCoords, std::vector<float> weigth, std::vector<uint8_t> ids, BonesList bonelist)
	: Mesh(nov, noi, vert, norm, ind, textureCoords), weights(weigth), bones(bonelist) , boneids(ids)
{
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::loadMesh()
{
	createVAO();
	storeData(0, numberOfIndices * 3, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_INT, indices, 3, true);
	storeData(0, numberOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, vertices, 3);
	storeData(1, numberOfVertices * 3, GL_ARRAY_BUFFER, GL_FLOAT, normals, 3);
	
	if (textureCoords)
		storeData(2, numberOfVertices * 2, GL_ARRAY_BUFFER, GL_FLOAT, textureCoords, 2);
	storeData(3, numberOfVertices * 4, GL_ARRAY_BUFFER, GL_FLOAT, &weights.data()[0], 4);
	storeData(4, numberOfVertices * 4, GL_ARRAY_BUFFER, GL_UNSIGNED_BYTE, &boneids.data()[0], 4);
	glBindVertexArray(0);
}
