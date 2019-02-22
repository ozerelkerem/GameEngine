#include "Renderer.h"



Renderer::Renderer(GameBase *gb)
{
	
	
	gamebase = gb;
}


Renderer::~Renderer()
{
}

void Renderer::renderModels()
{
	for (auto modelmap : this->gamebase->currentScene->componentSystem->actorsWhichContainsModelComponent)
	{
		for (int i = 0; i < modelmap.first->numOfMeshes; i++)
		{
			
			modelmap.first->meshes[i]->bind();

			for (auto actor : modelmap.second)
			{
				
				normalShader->setMat4("modelMatrix", actor->transformation->realMatrix);
				modelmap.first->meshes[i]->render();
			}

			modelmap.first->meshes[i]->unbind();
		}
		
	}
}
