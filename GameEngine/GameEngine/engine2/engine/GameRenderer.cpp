#include "GameRenderer.h"


#include<engine/ActorManager.h>
GameRenderer::GameRenderer(GameBase *b) : Renderer(b)
{
	GenerateBuffers();
}


GameRenderer::~GameRenderer()
{
}

void GameRenderer::render()
{
	if (gamebase->currentScene->cameraActor == ActorID::INVALID_HANDLE)
		return;
	

	Actor *camActor = GE_Engine->actorManager->GetActor(gamebase->currentScene->cameraActor);
	CameraComponent *mainCam = camActor->GetComponent<CameraComponent>();
	
	mainCam->updateViewMatrix();
	mainCam->updateprojectionMatrix();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, GE_Engine->screenSize.x, GE_Engine->screenSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(255.f, 255.f, 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	normalShader->Use();
	normalShader->setMat4("viewMatrix", mainCam->viewMatrix);
	normalShader->setMat4("projectionMatrix", mainCam->projectionMatrix);
	normalShader->setVec3("viewPos", camActor->transformation.getWorldPosition());

	prepareLights();
	renderModels();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
