#include "SceneTools.h"
#include "Model.h"


SceneTools::SceneTools()
{
	mode = SceneToolModes::MOVE;
	movemode = SceneToolMoveModes::MOVEX;
	cone = ModelFunctions::CreateBasicCone();
}


void SceneTools::Render(Transform * transform, float dis)
{
	RenderMove(transform, dis);
}

SceneTools::~SceneTools()
{
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
	switch (mode)
	{
	case ROTATE:
	{

	}
		break;
	case SCALE:
	{

	}
		break;
	case MOVE:
	{
		if (color[0] == 1.f)
		{
			initPos = transform->position;
			std::cout << "red";
			return true;
		}
		else if (color[1] == 1.f)
		{
			initPos = transform->position;
			std::cout << "gree";
			return true;
		}
		else if (color[2] == 1.f)
		{
			initPos = transform->position;
			std::cout << "bl";
			return true;
		}
	}
		break;

	}

	return false;
}

void SceneTools::transObjects(Transform * transform, int x, int y, int dx, int dy, glm::vec3 campos, glm::vec3 worldtoscreen)
{
	if (movemode == SceneToolMoveModes::MOVEX)
	{
		
		transform->position =  glm::vec3(worldtoscreen.x, 0, 0);
		
		
		
		transform->calcModelMatrix();
	}

}

