#include "SceneRenderer.h"



SceneRenderer::SceneRenderer(GameBase *b) : Renderer(b)
{
	sceneCamera = new SceneCamera(glm::vec3(20, -50, 20), 90, 0);
	GenerateBuffers();

	backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);

	sceneTool = new SceneTools(this);

	selectedActor = ActorID::INVALID_HANDLE;
	hoveredActor = ActorID::INVALID_HANDLE;

	grid = new Grid(16, 1, colorShader->getProgramID());
}


SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::render()
{
	Actor *selectedactor = GE_Engine->actorManager->GetActor(selectedActor);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, sceneSize.x, sceneSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneCamera->projectionMatrix = glm::perspective(30.0f, (float)(sceneSize.x / sceneSize.y), 0.1f, 1000.0f);
	sceneCamera->UpdateViewMatrix();
	
	colorShader->Use();
	colorShader->setMat4("viewMatrix", sceneCamera->viewMatrix);
	colorShader->setMat4("projectionMatrix", sceneCamera->projectionMatrix);
	colorShader->setMat4("modelMatrix", glm::mat4(1));
	grid->Draw();
	renderLights(this->sceneCamera);
	if (selectedactor)
		RenderOutlined(selectedactor);

	normalShader->Use();
	normalShader->setMat4("viewMatrix", sceneCamera->viewMatrix);
	normalShader->setMat4("projectionMatrix", sceneCamera->projectionMatrix);
	normalShader->setVec3("viewPos", sceneCamera->position);

	prepareLights();

	renderModels();
	
	colorShader->Use();
 if (selectedactor)
	{
		glDisable(GL_DEPTH_TEST);

		sceneTool->Render(selectedactor->transformation, glm::distance(sceneCamera->position, selectedactor->transformation->getWorldPosition()) / 10, sceneCamera->position);

		glEnable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::GenerateBuffers()
{
	// framebuffer configuration
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sceneMaxWidth, sceneMaxHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sceneMaxWidth, sceneMaxHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void SceneRenderer::RenderOutlined(Actor * o)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	normalShader->Use();
	normalShader->setMat4("modelMatrix", o->transformation->worldMatrix);
	RenderAnActor(o, normalShader);

	colorShader->Use();
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	colorShader->setVec3("color", glm::vec3(1, 1, 0));

	o->transformation->worldMatrix = glm::scale(o->transformation->worldMatrix, { 1.1, 1.1, 1.1 });
	colorShader->setMat4("modelMatrix", o->transformation->worldMatrix);
	RenderAnActor(o, colorShader);

	o->RecalculateRealMatrix();

	glDisable(GL_STENCIL_TEST);
}

void SceneRenderer::renderModelsColored()
{
	colorShader->Use();
	colorShader->setInt("hasBones", 0);
	for (auto modelmap : this->gamebase->currentScene->componentSystem->actorsWhichContainsModelComponent)
	{
		for (int i = 0; i < modelmap.first->numOfMeshes; i++)
		{
			modelmap.first->meshes[i]->bind();

			for (auto actorid : modelmap.second)
			{
				Actor *actor = GE_Engine->actorManager->GetActor(actorid);
				ModelComponent *mcmp = actor->componentObject->getComponent<ModelComponent>();

				ActorID test = actorid;
				float r = (test.index & 0x000000FF) >> 0;
				float g = (test.index & 0x0000FF00) >> 8;
				float b = (test.index & 0x00FF0000) >> 16;
				colorShader->setVec3("color", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));

				colorShader->setMat4("modelMatrix", actor->transformation->worldMatrix);
				modelmap.first->meshes[i]->render();	
			}

			modelmap.first->meshes[i]->unbind();
		}
	}

	colorShader->setInt("hasBones", 1);
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
					ActorID test = actorid;
					float r = (test.index & 0x000000FF) >> 0;
					float g = (test.index & 0x0000FF00) >> 8;
					float b = (test.index & 0x00FF0000) >> 16;
					colorShader->setVec3("color", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));

					std::vector<glm::mat4> matrixBuffer;
					int j = 0;
					for (auto x : mcmp->effectlist[i])
					{
						matrixBuffer.push_back((GE_Engine->actorManager->GetActor(x)->transformation->worldMatrix) * glm::transpose(((SkinnedMesh*)modelmap.first->meshes[i])->bones[j].second));
						j++;
					}
					colorShader->setMat4Array("bones", matrixBuffer.size(), matrixBuffer.data()[0]);

					modelmap.first->meshes[i]->render();
				}

			
			}

			modelmap.first->meshes[i]->unbind();
		}
	}
}

ActorID SceneRenderer::RenderForObjectPicker(GLint x, GLint y)
{
	colorShader->Use();
	glViewport(0, 0, sceneSize.x, sceneSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	renderModelsColored();

	unsigned char test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &test);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (test[0] == 0 && test[1] == 0 && test[2] == 0)
		return ActorID::INVALID_HANDLE;
	else
		return GE_Engine->actorManager->GetActorID(test[0] + test[1] * 256 + test[2] * 256 * 256);
}

bool SceneRenderer::RenderForObjectTools(GLint x, GLint y)
{
	Actor * selectedactor = GE_Engine->actorManager->GetActor(selectedActor);
	if (!selectedactor)
		return false;
	colorShader->Use();
	glViewport(0, 0, sceneSize.x, sceneSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneTool->Render(selectedactor->transformation, glm::distance(sceneCamera->position, selectedactor->transformation->getWorldPosition()) / 10, sceneCamera->position);

	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return sceneTool->processTool(test, selectedactor->transformation);
}

void SceneRenderer::focusActor(ActorID actorid)
{
	//@TODO CALC MÝN MAX FOR EACH MESH AND CHECK MAYBE THERE IS NO MODEL
	Actor * actor = GE_Engine->actorManager->GetActor(actorid);
	if (!actor->componentObject->hasComponent<ModelComponent>())
		return;

	ModelComponent *mcmp = actor->componentObject->getComponent<ModelComponent>();
	auto x = mcmp->model->meshes[0]->bounds;

	glm::vec4 minxyz(x.minx, x.miny, x.minz, 1.0);
	glm::vec4 maxxyz(x.maxx, x.maxy, x.maxz, 1.0);

  	minxyz = actor->transformation->worldMatrix * minxyz;
	maxxyz = actor->transformation->worldMatrix *maxxyz;
	
	minxyz = minxyz * (float)(1.0 / minxyz.w);
	maxxyz = maxxyz * (float)(1.0 / maxxyz.w);

	float dis = glm::distance(minxyz, maxxyz);


	sceneCamera->pitch = -25;
	sceneCamera->yaw = 145;
	glm::vec3 look(1, -1, 1);
	glm::normalize(look);
	look *=  dis;

	sceneCamera->position = actor->transformation->getWorldPosition() + look;
	sceneCamera->UpdateCameraVectors();
	
}


