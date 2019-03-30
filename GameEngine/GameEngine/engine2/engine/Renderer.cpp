#include "Renderer.h"
#include<engine/Actor.h>
#include<engine/ActorManager.h>
#include <editor/SceneCamera.h>
#include <editor/ConstantModels.h>
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
	ModelComponent *m = actor->GetComponent<ModelComponent>();
	SkinnedModelComponent *sm  = actor->GetComponent<SkinnedModelComponent>();
	if (m)
	{
		shader->setInt("hasBones", 0);
		for (int i = 0; i < m->getModel()->numOfMeshes; i++)
		{
			m->getModel()->meshes[i]->bind();
			m->getModel()->meshes[i]->render();
			m->getModel()->meshes[i]->unbind();
		}
	}
	else if(sm)
	{
		shader->setInt("hasBones", 1);
		for (int i = 0; i < sm->getModel()->numOfMeshes; i++)
		{
			sm->getModel()->meshes[i]->bind();

			if (sm->rootBone != ActorID::INVALID_HANDLE)
			{
				std::vector<glm::mat4> matrixBuffer;
				int j = 0;
				for (auto x : sm->effectlist[i])
				{
					matrixBuffer.push_back((GE_Engine->actorManager->GetActor(x)->transformation->worldMatrix) * glm::transpose(((SkinnedMesh*)sm->getModel()->meshes[i])->bones[j].second));
					if(shader == colorShader &&  j==0)
						matrixBuffer[0]= glm::scale(matrixBuffer[0], { 1.1, 1.1, 1.1 });
					
					j++;
				}
				shader->setMat4Array("bones", matrixBuffer.size(), matrixBuffer.data()[0]);

				sm->getModel()->meshes[i]->render();
			}

			sm->getModel()->meshes[i]->unbind();
		}
	}
}

void Renderer::renderModels()
{
	normalShader->setInt("hasBones", 0);
	auto mend = GE_Engine->componentManager->end<ModelComponent>();
	for (auto model = GE_Engine->componentManager->begin<ModelComponent>(); model.operator!=(mend); model.operator++())
	{
		if (!model->getModel())
			continue;
		for (int i = 0; i < model->getModel()->numOfMeshes; i++)
		{
			if (model->numberOfMaterials > i && model->materials[i])
				model->materials[i]->active();
			else
				ConstantMaterials::Materials::defaultMaterial->active();
			normalShader->setMat4("modelMatrix", GE_Engine->actorManager->GetActor(model->owner)->transformation->worldMatrix);
			model->getModel()->meshes[i]->bind();
			model->getModel()->meshes[i]->render();
			model->getModel()->meshes[i]->unbind();
			glActiveTexture(GL_TEXTURE0);
		}
	}

	normalShader->setInt("hasBones", 1);
	auto send = GE_Engine->componentManager->end<SkinnedModelComponent>();
	for (auto model = GE_Engine->componentManager->begin<SkinnedModelComponent>(); model.operator!=(send); model.operator++())
	{
		if (!model->getModel())
			continue;
		for (int i = 0; i < model->getModel()->numOfMeshes; i++)
		{
			Actor *actor = GE_Engine->actorManager->GetActor(model->owner);

			if (model->rootBone != ActorID::INVALID_HANDLE)
			{
				if (model->numberOfMaterials > i && model->materials[i])
					model->materials[i]->active();
				else
					ConstantMaterials::Materials::defaultMaterial->active();
				model->getModel()->meshes[i]->bind();
				std::vector<glm::mat4> matrixBuffer;
				int j = 0;
				for (auto x : model->effectlist[i])
				{
					matrixBuffer.push_back((GE_Engine->actorManager->GetActor(x)->transformation->worldMatrix) * glm::transpose(((SkinnedMesh*)model->getModel()->meshes[i])->bones[j].second));
					j++;
				}
				normalShader->setMat4Array("bones", matrixBuffer.size(), matrixBuffer.data()[0]);
				model->getModel()->meshes[i]->render();
				model->getModel()->meshes[i]->unbind();
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}
}


void Renderer::prepareLights()
{

	int maxlight = 7;
	int index = 0;
	auto end = GE_Engine->componentManager->end<LightComponent>();
	for (auto light = GE_Engine->componentManager->begin<LightComponent>(); light.operator!=(end);  light.operator++())
	{
		light->passShader(normalShader, GE_Engine->actorManager->GetActor(light->owner)->transformation, index);
		index++;
	} 
	while (index <= maxlight)
	{
		normalShader->setInt("lights[" + std::to_string(index) + "].type", 5);
		index++;
	}

}
