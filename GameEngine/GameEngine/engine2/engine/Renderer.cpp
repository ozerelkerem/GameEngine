#include "Renderer.h"
#include<engine/Actor.h>
#include<engine/ActorManager.h>

Renderer::Renderer(GameBase *gb)
{
	
	
	gamebase = gb;
}


Renderer::~Renderer()
{
}

void Renderer::RenderAnActor(Model *model)
{
	colorShader->setInt("hasBones",0);
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

void Renderer::RenderAnActor(Actor *actor, Shader *shader)
{
	ModelComponent *m = actor->componentObject->getComponent<ModelComponent>();
	SkinnedModelComponent *sm  = actor->componentObject->getComponent<SkinnedModelComponent>();
	if (m)
	{
		shader->setInt("hasBones", 0);
		for (int i = 0; i < m->model->numOfMeshes; i++)
		{
			m->model->meshes[i]->bind();
			m->model->meshes[i]->render();
			m->model->meshes[i]->unbind();
		}
	}
	else if(sm)
	{
		shader->setInt("hasBones", 1);
		for (int i = 0; i < sm->model->numOfMeshes; i++)
		{
			sm->model->meshes[i]->bind();

			if (sm->rootBone != ActorID::INVALID_HANDLE)
			{
				std::vector<glm::mat4> matrixBuffer;
				int j = 0;
				for (auto x : sm->effectlist[i])
				{
					matrixBuffer.push_back((GE_Engine->actorManager->GetActor(x)->transformation->worldMatrix) * glm::transpose(((SkinnedMesh*)sm->model->meshes[i])->bones[j].second));
					if(shader == colorShader &&  j==0)
						matrixBuffer[0]= glm::scale(matrixBuffer[0], { 1.1, 1.1, 1.1 });
					
					j++;
				}
				shader->setMat4Array("bones", matrixBuffer.size(), matrixBuffer.data()[0]);

				sm->model->meshes[i]->render();
			}

			sm->model->meshes[i]->unbind();
		}
	}
}

void Renderer::renderModels()
{
	normalShader->setInt("hasBones", 0);
	for (auto modelmap : this->gamebase->currentScene->componentSystem->actorsWhichContainsModelComponent)
	{
		for (int i = 0; i < modelmap.first->numOfMeshes; i++)
		{
			modelmap.first->meshes[i]->bind();

			for (auto actorid : modelmap.second)
			{
				Actor *actor = GE_Engine->actorManager->GetActor(actorid);
				ModelComponent *mcmp = actor->componentObject->getComponent<ModelComponent>();
				
				if (mcmp->numberOfMaterials > i && mcmp->materials[i])
					mcmp->materials[i]->active();
				else
					Material::noMaterial();
				
				normalShader->setMat4("modelMatrix", actor->transformation->worldMatrix);
				modelmap.first->meshes[i]->render();
				
				glActiveTexture(GL_TEXTURE0);
			}

			modelmap.first->meshes[i]->unbind();
		}
	}

	normalShader->setInt("hasBones", 1);
	for (auto modelmap : this->gamebase->currentScene->componentSystem->actorsWhichContainsSkinnedModelComponent)
	{
		for (int i = 0; i < modelmap.first->numOfMeshes; i++)
		{
			modelmap.first->meshes[i]->bind();

			for (auto actorid : modelmap.second)
			{
				Actor *actor = GE_Engine->actorManager->GetActor(actorid);
				SkinnedModelComponent *mcmp = actor->componentObject->getComponent<SkinnedModelComponent>();
				
				if (mcmp->rootBone != ActorID::INVALID_HANDLE)
				{
					if (mcmp->numberOfMaterials > i && mcmp->materials[i])
						mcmp->materials[i]->active();
					else
						Material::noMaterial();

					std::vector<glm::mat4> matrixBuffer;
					int j = 0;
					for (auto x : mcmp->effectlist[i])
					{
					
						matrixBuffer.push_back( (GE_Engine->actorManager->GetActor(x)->transformation->worldMatrix) * glm::transpose(((SkinnedMesh*)modelmap.first->meshes[i])->bones[j].second));
						j++;
					}
					normalShader->setMat4Array("bones", matrixBuffer.size() , matrixBuffer.data()[0]);

					modelmap.first->meshes[i]->render();
				}

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
		lightactor.second->passShader(normalShader, GE_Engine->actorManager->GetActor(lightactor.first)->transformation, maxlight);
		if (maxlight-- < 0)
			break;
	} 
	while (maxlight-- > 0)
	{
		normalShader->setInt("lights[" + std::to_string(maxlight) + "].type", 5);
	}

}
