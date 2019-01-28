#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

extern Shader* colorShader;

enum SceneToolModes
{
	ROTATE, SCALE, MOVE
};

class SceneTools
{
public:
	SceneTools();

	SceneToolModes mode;

	void Render(Transform *transform, float);


	bool processTool(GLfloat *color, Transform * transform);

	~SceneTools();

	

private:

	Mesh *cone, *cube, *sphere;

	void RenderRotate(Transform * transform);
	void RenderScale(Transform * transform);
	void RenderMove(Transform * transform, float);

};

