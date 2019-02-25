#include "SceneRenderer.h"



SceneRenderer::SceneRenderer(GameBase *b) : Renderer(b)
{
	sceneCamera = new SceneCamera(glm::vec3(20, -50, 20), 90, 0);
	GenerateBuffers();

	backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);

	sceneTool = new SceneTools(this);

	selectedActor = NULL;
	hoveredActor = NULL;

	grid = new Grid(16, 1, colorShader->getProgramID());
}


SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::render()
{
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
	if (selectedActor)
		RenderOutlined(selectedActor);

	normalShader->Use();
	normalShader->setMat4("viewMatrix", sceneCamera->viewMatrix);
	normalShader->setMat4("projectionMatrix", sceneCamera->projectionMatrix);

	renderModels();
	
	colorShader->Use();
	if (selectedActor)
	{
		glDisable(GL_DEPTH_TEST);

		sceneTool->Render(selectedActor->transformation, glm::distance(sceneCamera->position, selectedActor->transformation->position) / 10, sceneCamera->position);

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
	normalShader->setMat4("modelMatrix", o->transformation->realMatrix);
	RenderAnActor(o);

	colorShader->Use();
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	colorShader->setVec3("color", glm::vec3(1, 1, 0));

	o->transformation->realMatrix = glm::scale(o->transformation->realMatrix, { 1.1, 1.1, 1.1 });
	colorShader->setMat4("modelMatrix", o->transformation->realMatrix);
	RenderAnActor(o);

	o->RecalculateRealMatrix();

	glDisable(GL_STENCIL_TEST);
}

Actor * SceneRenderer::RenderForObjectPicker(GLint x, GLint y)
{
	colorShader->Use();
	glViewport(0, 0, sceneSize.x, sceneSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	for (auto modelmap : this->gamebase->currentScene->componentSystem->actorsWhichContainsModelComponent)
	{
		for (int i = 0; i < modelmap.first->numOfMeshes; i++)
		{
			modelmap.first->meshes[i]->bind();

			for (auto actor : modelmap.second)
			{
				colorShader->setVec3("color", glm::vec3(actor->id / 255.f, 0, 0));

				normalShader->setMat4("modelMatrix", actor->transformation->realMatrix);
				modelmap.first->meshes[i]->render();
			}

			modelmap.first->meshes[i]->unbind();
		}
	}

	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (test[0] == 0)
		return NULL;
	else
		return gamebase->currentScene->getActorByID((int)(test[0] * 255));
}

bool SceneRenderer::RenderForObjectTools(GLint x, GLint y)
{
	if (!selectedActor)
		return false;
	colorShader->Use();
	glViewport(0, 0, sceneSize.x, sceneSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneTool->Render(selectedActor->transformation, glm::distance(sceneCamera->position, selectedActor->transformation->position) / 10, sceneCamera->position);

	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return sceneTool->processTool(test, selectedActor->transformation);
}


