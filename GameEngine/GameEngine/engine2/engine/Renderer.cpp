#include "Renderer.h"



Renderer::Renderer(GameBase *gb)
{
	
	
	gamebase = gb;
}


Renderer::~Renderer()
{
}

void Renderer::RenderAnActor(Model *model)
{
	if (model)
	{
		for (int i = 0; i < model->numOfMeshes; i++)
		{
			model->meshes[i]->bind();
			model->meshes[i]->render();
			model->meshes[i]->unbind();
		}
	}
}

void Renderer::RenderAnActor(Actor *actor)
{
	ModelComponent *m = static_cast<ModelComponent *>(actor->componentObject->getComponentByComponentType(ComponentType::ModelComp));

	if (m)
	{
		for (int i = 0; i < m->model->numOfMeshes; i++)
		{
			m->model->meshes[i]->bind();
			m->model->meshes[i]->render();
			m->model->meshes[i]->unbind();
		}
	}
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
