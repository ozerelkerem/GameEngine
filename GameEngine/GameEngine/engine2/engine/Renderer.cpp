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
				ModelComponent *mcmp = (ModelComponent*)actor->componentObject->getComponentByComponentType(ComponentType::ModelComp);
				if (mcmp->numberOfMaterials > i && mcmp->materials[i])
					mcmp->materials[i]->active();
				else
					Material::noMaterial();
				normalShader->setMat4("modelMatrix", actor->transformation->realMatrix);
				modelmap.first->meshes[i]->render();
				
				glActiveTexture(GL_TEXTURE0);
			}

			modelmap.first->meshes[i]->unbind();
		}
	}
}

void Renderer::prepareLights()
{

	int maxlight = 7;
	for (auto lightactor : this->gamebase->currentScene->componentSystem->actorsWhichContainsLightComponent)
	{
		lightactor.second->passShader(normalShader, lightactor.first->transformation, maxlight);
		if (maxlight-- < 0)
			break;
	} 
	while (maxlight-- > 0)
	{
		normalShader->setInt("lights[" + std::to_string(maxlight) + "].type", 5);
	}

}
