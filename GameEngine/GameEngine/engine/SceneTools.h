#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

extern Shader* colorShader;

enum SceneToolModes
{
	ROTATE, SCALE, MOVE
};

enum SceneToolMoveModes
{
	MOVEXY, MOVEXZ, MOVEYZ, MOVEX, MOVEY, MOVEZ
};
class SceneTools
{
public:
	SceneTools();

	SceneToolModes mode;
	SceneToolMoveModes movemode;

	void Render(Transform *transform, float);
	

	bool processTool(GLfloat *color, Transform * transform);
	void transObjects(Transform * transform, int x, int y, int dx, int dy, glm::vec3 campos, glm::vec3 worldtoscreen);

	~SceneTools();

	

private:

	Mesh *cone, *cube, *sphere;

	glm::vec3 initPos;

	void RenderRotate(Transform * transform);
	void RenderScale(Transform * transform);
	void RenderMove(Transform * transform, float);

};

