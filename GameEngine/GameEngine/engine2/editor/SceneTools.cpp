#include "SceneTools.h"



SceneTools::SceneTools(SceneRenderer *sr)
{
	sceneRenderer = sr;

	mode = SceneToolModes::ROTATE;
	modedirection = 0;
	translateTool = ConstantModels::CreateTranslateTool();
	rotateTool = ConstantModels::CreateRotateTool();
	scaleTool = ConstantModels::CreateScaleTool();


}


void SceneTools::Render(Transform * transform, float dis, glm::vec3  &camera)
{
	switch (mode)
	{
	case ROTATE:
		RenderRotate(transform, dis, camera);
		break;
	case SCALE:
		RenderScale(transform, dis*0.9);
		break;
	case MOVE:
		RenderMove(transform, dis);
		break;
	}
}

SceneTools::~SceneTools()
{
}

void SceneTools::RenderRotate(Transform * transform, float dis, glm::vec3 &camerapos)
{
	float rY = 0;
	float rX = 270.f;
	float rZ = 90.f;
	if (camerapos.x < transform->position.x && camerapos.z > transform->position.z)
		rY = -90.f;
	else if (camerapos.x < transform->position.x && camerapos.z < transform->position.z)
		rY = 180.f;
	else if (camerapos.x > transform->position.x && camerapos.z < transform->position.z)
		rY = 90.f;


	if (camerapos.y < transform->position.y && camerapos.z > transform->position.z)
		rX = 0.f;
	else if (camerapos.y < transform->position.y && camerapos.z < transform->position.z)
		rX = 90.f;
	else if (camerapos.y > transform->position.y && camerapos.z < transform->position.z)
		rX = 180.f;

	if (camerapos.y < transform->position.y && camerapos.x > transform->position.x)
		rZ = 0.f;
	else if (camerapos.y < transform->position.y && camerapos.x < transform->position.x)
		rZ = -90.f;
	else if (camerapos.y > transform->position.y && camerapos.x < transform->position.x)
		rZ = 180.f;


	glm::vec3 scale(dis, dis, dis);
	
	colorShader->Use();

	glm::vec3 pos, a, b;
	glm::vec4 c;
	glm::quat d(1, 1, 1, 1);
	glm::decompose(transform->realMatrix, a, d, pos, b, c);

	colorShader->setVec3("color", glm::vec3(1, 0, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::rotate(glm::translate(pos), glm::radians(rX), glm::vec3(1, 0, 0)), glm::radians(-90.f), glm::vec3(0, 0, 1)), scale));

	sceneRenderer->RenderAnActor(rotateTool);

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(pos), glm::radians(rY), glm::vec3(0.f, 1, 0)), scale));

	sceneRenderer->RenderAnActor(rotateTool);

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::rotate(glm::translate(pos), glm::radians(rZ), glm::vec3(0, 0, 1)), glm::radians(90.f), glm::vec3(1, 0, 0)), scale));

	sceneRenderer->RenderAnActor(rotateTool);
}

void SceneTools::RenderScale(Transform * transform, float dis)
{
	glm::vec3 scale(dis, dis, dis);
	colorShader->Use();
	colorShader->setVec3("color", glm::vec3(1, 0, 0));

	glm::vec3 pos, a, b;
	glm::vec4 c;
	glm::quat rot(1, 1, 1, 1);
	glm::decompose(transform->realMatrix, a, rot, pos, b, c);

	colorShader->setMat4("modelMatrix", glm::scale(glm::translate(pos) * glm::toMat4(rot), scale));

	sceneRenderer->RenderAnActor(scaleTool);

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(pos) * glm::toMat4(rot), glm::radians(90.f), glm::vec3(0.f, 0, 1)), scale));

	sceneRenderer->RenderAnActor(scaleTool);

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(pos)  * glm::toMat4(rot), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)), scale));

	sceneRenderer->RenderAnActor(scaleTool);
}

void SceneTools::RenderMove(Transform * transform, float dis)
{
	glm::vec3 scale(dis, dis, dis);
	colorShader->Use();
	colorShader->setVec3("color", glm::vec3(1, 0, 0));

	glm::vec3 pos, a, b;
	glm::vec4 c;
	glm::quat d(1,1,1,1);
	glm::decompose(transform->realMatrix,a, d, pos, b, c);

	colorShader->setMat4("modelMatrix", glm::scale(glm::translate(pos), scale));
	sceneRenderer->RenderAnActor(translateTool);

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(pos), glm::radians(90.f), glm::vec3(0.f, 0, 1)), scale));

	sceneRenderer->RenderAnActor(translateTool);

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(pos), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)), scale));
	sceneRenderer->RenderAnActor(translateTool);

}

bool SceneTools::processTool(GLfloat *color, Transform * transform)
{
	modedirection = 0;
	if (color[0] == 1.f)
		modedirection |= TOOLX;
	if (color[1] == 1.f)
		modedirection |= TOOLY;
	if (color[2] == 1.f)
		modedirection |= TOOLZ;

	initPos = transform->position;

	if (modedirection)
		return true;
	return false;
}

void SceneTools::transObjects(Transform * transform, int x, int y, int dx, int dy, glm::vec3 campos, glm::vec3 worldtoscreen)
{

	switch (mode)
	{
	case ROTATE:
	{
		if (modedirection & TOOLX)
		{
			transform->eRotation.x += dx;
		}
		if (modedirection & TOOLY)
		{
			transform->eRotation.y += dx / 4;
		}
		if (modedirection & TOOLZ)
		{
			transform->eRotation.z += dx;
		}
		transform->calcQuatFromEuler();
//		transform->calcModelMatrix();
	}

	break;
	case SCALE:
	{
		if (modedirection & TOOLX)
		{
			transform->scale.x += dx;
		}
		if (modedirection & TOOLY)
		{
			transform->scale.y += dx;
		}
		if (modedirection & TOOLZ)
		{
			transform->scale.z += dx;
		}

//		transform->calcModelMatrix();
	}
	break;
	case MOVE:
	{
		if (modedirection & TOOLX)
		{
			transform->position.x = worldtoscreen.x;
		}
		if (modedirection & TOOLY)
		{
			transform->position.y = worldtoscreen.y;
		}
		if (modedirection & TOOLZ)
		{
			transform->position.z = worldtoscreen.z;
		}

		if (glm::distance(transform->position.x, campos.x) > 150.f)
			transform->position.x = initPos.x;
		if (glm::distance(transform->position.y, campos.y) > 150.f)
			transform->position.y = initPos.y;
		if (glm::distance(transform->position.z, campos.z) > 150.f)
			transform->position.z = initPos.z;
		initPos = transform->position;
//		transform->calcModelMatrix();
	}
	break;

	}



}

