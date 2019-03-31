#include "MatConverter.h"

PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4)
{
	PxMat44 newMat;

	newMat[0][0] = mat4[0][0];
	newMat[0][1] = mat4[0][1];
	newMat[0][2] = mat4[0][2];
	newMat[0][3] = mat4[0][3];

	newMat[1][0] = mat4[1][0];
	newMat[1][1] = mat4[1][1];
	newMat[1][2] = mat4[1][2];
	newMat[1][3] = mat4[1][3];

	newMat[2][0] = mat4[2][0];
	newMat[2][1] = mat4[2][1];
	newMat[2][2] = mat4[2][2];
	newMat[2][3] = mat4[2][3];

	newMat[3][0] = mat4[3][0];
	newMat[3][1] = mat4[3][1];
	newMat[3][2] = mat4[3][2];
	newMat[3][3] = mat4[3][3];


	return newMat;
}