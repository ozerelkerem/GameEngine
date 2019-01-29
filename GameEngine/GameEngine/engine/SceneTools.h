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
	TOOLX=1, TOOLY=2, TOOLZ=4
};
class SceneTools
{
public:
	SceneTools();

	SceneToolModes mode;
	unsigned int modedirection;

	void Render(Transform *transform, float);
	

	bool processTool(GLfloat *color, Transform * transform);
	void transObjects(Transform * transform, int x, int y, int dx, int dy, glm::vec3 campos, glm::vec3 worldtoscreen);

	~SceneTools();

	

private:

	Mesh *cone, *cube, *sphere;

	glm::vec3 initPos;

	void RenderRotate(Transform * transform, float dis);
	void RenderScale(Transform * transform, float dis);
	void RenderMove(Transform * transform, float);

};

