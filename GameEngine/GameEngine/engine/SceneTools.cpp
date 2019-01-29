#include "SceneTools.h"
#include "Model.h"


SceneTools::SceneTools()
{
	mode = SceneToolModes::ROTATE;
	modedirection = 0;
	cone = ModelFunctions::CreateBasicCone();
	cube = ModelFunctions::CreateCubeArm();
	sphere = ModelFunctions::CreateQuarterCircle();
}


void SceneTools::Render(Transform * transform, float dis)
{
	switch (mode)
	{
	case ROTATE:
		RenderRotate(transform, dis);
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

void SceneTools::RenderRotate(Transform * transform, float dis)
{
	glm::vec3 scale(dis, dis, dis);
	colorShader->Use();
	colorShader->setVec3("color", glm::vec3(1, 0, 0));

	colorShader->setMat4("modelMatrix", glm::scale(glm::translate(transform->position), scale));
	sphere->Render();

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(90.f), glm::vec3(0.f, 0, 1)), scale));

	sphere->Render();

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)), scale));
	sphere->Render();
}

void SceneTools::RenderScale(Transform * transform, float dis)
{
	glm::vec3 scale(dis, dis, dis);
	colorShader->Use();
	colorShader->setVec3("color", glm::vec3(1, 0, 0));

	colorShader->setMat4("modelMatrix", glm::scale(glm::translate(transform->position), scale));
	cube->Render();

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(90.f), glm::vec3(0.f, 0, 1)), scale));

	cube->Render();

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)), scale));
	cube->Render();
}

void SceneTools::RenderMove(Transform * transform, float dis)
{
	glm::vec3 scale(dis, dis, dis);
	colorShader->Use();
	colorShader->setVec3("color", glm::vec3(1,0,0));	

	colorShader->setMat4("modelMatrix", glm::scale(glm::translate(transform->position), scale));
	cone->Render();

	colorShader->setVec3("color", glm::vec3(0, 1, 0));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(90.f), glm::vec3(0.f, 0, 1)),scale));

	cone->Render();

	colorShader->setVec3("color", glm::vec3(0, 0, 1));
	colorShader->setMat4("modelMatrix", glm::scale(glm::rotate(glm::translate(transform->position), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f)),scale));
	cone->Render();

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
		
		break;
	case SCALE:
		{
			
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
		transform->calcModelMatrix();
	}
		break;
	
	}
	

	
}

