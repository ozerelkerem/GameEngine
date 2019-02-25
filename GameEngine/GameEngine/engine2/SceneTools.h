#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

#include <ConstantModels.h>
#include <SceneRenderer.h>

extern Shader* colorShader;

class SceneRenderer;

enum SceneToolModes
{
	ROTATE, SCALE, MOVE
};

enum SceneToolMoveModes
{
	TOOLX = 1, TOOLY = 2, TOOLZ = 4
};
class SceneTools
{
private:

	Model *translateTool, *rotateTool, *scaleTool;

	glm::vec3 initPos;

	void RenderRotate(Transform * transform, float dis, glm::vec3 &);
	void RenderScale(Transform * transform, float dis);
	void RenderMove(Transform * transform, float);

	SceneRenderer *sceneRenderer;

public:
	SceneTools(SceneRenderer *);

	SceneToolModes mode;
	unsigned int modedirection;

	bool processTool(GLfloat *color, Transform * transform);
	void transObjects(Transform * transform, int x, int y, int dx, int dy, glm::vec3 campos, glm::vec3 worldtoscreen);

	void Render(Transform * transform, float dis, glm::vec3 &);

	~SceneTools();


};

