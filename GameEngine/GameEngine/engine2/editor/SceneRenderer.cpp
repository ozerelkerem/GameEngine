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

	spriteRenderer = new SpriteRenderer();
}

void SceneRenderer::renderSelectedCamera()
{
	if (Actor *cameraActor = GE_Engine->actorManager->GetActor(selectedActor); cameraActor && cameraActor->GetComponent<CameraComponent>())
	{
		CameraComponent *cc = cameraActor->GetComponent<CameraComponent>();

		colorShader->setMat4("modelMatrix", glm::mat4(1));
		colorShader->setInt("hasBones", 0);
		colorShader->setVec3("color", glm::vec3(0.6f, 0.6f, 0.6f));

		glBegin(GL_LINES);

		float fov = glm::radians(cc->fov);
		glm::vec3 pos = cameraActor->transformation.getWorldPosition();
		glm::vec3 up = cameraActor->transformation.getWorldUpVector()*sceneSize.y;
		glm::vec3 forward = cameraActor->transformation.getWorldForwardVector();
		glm::vec3 right = cameraActor->transformation.getWorldRightVector() *sceneSize.x;

		if(cc->Perspective)
		{
			glm::vec3 nearPosition = pos + forward * cc->m_near;

			glm::vec3 near1 = glm::rotate(nearPosition - pos, fov, glm::normalize(right + up)) + pos;
			glm::vec3 near2 = glm::rotate(nearPosition - pos, fov, glm::normalize(right - up)) + pos;
			glm::vec3 near3 = glm::rotate(nearPosition - pos, fov, glm::normalize(-right - up)) + pos;
			glm::vec3 near4 = glm::rotate(nearPosition - pos, fov, glm::normalize(-right + up)) + pos;


			glVertexAttrib3f(0, near1.x, near1.y, near1.z);
			glVertexAttrib3f(0, near2.x, near2.y, near2.z);

			glVertexAttrib3f(0, near2.x, near2.y, near2.z);
			glVertexAttrib3f(0, near3.x, near3.y, near3.z);

			glVertexAttrib3f(0, near3.x, near3.y, near3.z);
			glVertexAttrib3f(0, near4.x, near4.y, near4.z);

			glVertexAttrib3f(0, near4.x, near4.y, near4.z);
			glVertexAttrib3f(0, near1.x, near1.y, near1.z);
		
			glm::vec3 farPosition = pos + forward * cc->m_far;

			glm::vec3 far1 = glm::rotate(farPosition - pos, fov, glm::normalize(right + up)) + pos;
			glm::vec3 far2 = glm::rotate(farPosition - pos, fov, glm::normalize(right - up)) + pos;
			glm::vec3 far3 = glm::rotate(farPosition - pos, fov, glm::normalize(-right - up)) + pos;
			glm::vec3 far4 = glm::rotate(farPosition - pos, fov, glm::normalize(-right + up)) + pos;


			glVertexAttrib3f(0, far1.x, far1.y, far1.z);
			glVertexAttrib3f(0, far2.x, far2.y, far2.z);

			glVertexAttrib3f(0, far2.x, far2.y, far2.z);
			glVertexAttrib3f(0, far3.x, far3.y, far3.z);

			glVertexAttrib3f(0, far3.x, far3.y, far3.z);
			glVertexAttrib3f(0, far4.x, far4.y, far4.z);

			glVertexAttrib3f(0, far4.x, far4.y, far4.z);
			glVertexAttrib3f(0, far1.x, far1.y, far1.z);



			glVertexAttrib3f(0, near1.x, near1.y, near1.z);
			glVertexAttrib3f(0, far1.x, far1.y, far1.z);

			glVertexAttrib3f(0, near2.x, near2.y, near2.z);
			glVertexAttrib3f(0, far2.x, far2.y, far2.z);

			glVertexAttrib3f(0, near3.x, near3.y, near3.z);
			glVertexAttrib3f(0, far3.x, far3.y, far3.z);

			glVertexAttrib3f(0, near4.x, near4.y, near4.z);
			glVertexAttrib3f(0, far4.x, far4.y, far4.z);
		}
		else
		{
			glm::vec3 nearPosition = pos + forward * cc->m_near;

			glm::vec3 near1 = nearPosition + cc->size * glm::normalize(right + up);
			glm::vec3 near2 = nearPosition + cc->size * glm::normalize(right - up);
			glm::vec3 near3 = nearPosition + cc->size * glm::normalize(-right - up);
			glm::vec3 near4 = nearPosition + cc->size * glm::normalize(-right + up);


			glVertexAttrib3f(0, near1.x, near1.y, near1.z);
			glVertexAttrib3f(0, near2.x, near2.y, near2.z);

			glVertexAttrib3f(0, near2.x, near2.y, near2.z);
			glVertexAttrib3f(0, near3.x, near3.y, near3.z);

			glVertexAttrib3f(0, near3.x, near3.y, near3.z);
			glVertexAttrib3f(0, near4.x, near4.y, near4.z);

			glVertexAttrib3f(0, near4.x, near4.y, near4.z);
			glVertexAttrib3f(0, near1.x, near1.y, near1.z);

			glm::vec3 farPosition = pos + forward * cc->m_far;

			glm::vec3 far1 = farPosition + cc->size * glm::normalize(right + up);
			glm::vec3 far2 = farPosition + cc->size * glm::normalize(right - up);
			glm::vec3 far3 = farPosition + cc->size * glm::normalize(-right - up);
			glm::vec3 far4 = farPosition + cc->size * glm::normalize(-right + up);


			glVertexAttrib3f(0, far1.x, far1.y, far1.z);
			glVertexAttrib3f(0, far2.x, far2.y, far2.z);

			glVertexAttrib3f(0, far2.x, far2.y, far2.z);
			glVertexAttrib3f(0, far3.x, far3.y, far3.z);

			glVertexAttrib3f(0, far3.x, far3.y, far3.z);
			glVertexAttrib3f(0, far4.x, far4.y, far4.z);

			glVertexAttrib3f(0, far4.x, far4.y, far4.z);
			glVertexAttrib3f(0, far1.x, far1.y, far1.z);



			glVertexAttrib3f(0, near1.x, near1.y, near1.z);
			glVertexAttrib3f(0, far1.x, far1.y, far1.z);

			glVertexAttrib3f(0, near2.x, near2.y, near2.z);
			glVertexAttrib3f(0, far2.x, far2.y, far2.z);

			glVertexAttrib3f(0, near3.x, near3.y, near3.z);
			glVertexAttrib3f(0, far3.x, far3.y, far3.z);

			glVertexAttrib3f(0, near4.x, near4.y, near4.z);
			glVertexAttrib3f(0, far4.x, far4.y, far4.z);
		}
		
		glEnd();
	}


	

	if (Actor *lightActor = GE_Engine->actorManager->GetActor(selectedActor); lightActor && lightActor->GetComponent<LightComponent>())
	{

		LightComponent *lightcomponent = lightActor->GetComponent<LightComponent>();


		const glm::vec3 lightpos = lightActor->transformation.getWorldPosition();
		float dist = lightcomponent->distance;

		const int sensivity = 36;
		float constexpr degree = 360 / sensivity;

		if (lightcomponent->lightType == LightType::Point)
		{
			renderSphere(lightpos, dist, 18);
		}
		else if (lightcomponent->lightType == LightType::Spotlight)
		{
			glm::vec3 circlepos = lightpos + (lightActor->transformation.getWorldForwardVector() * dist);
			glm::vec3 startpos = circlepos + (lightActor->transformation.getWorldUpVector() * dist *std::tan(glm::radians(lightcomponent->angle)));
			glm::vec3 endpos = glm::rotate((startpos - circlepos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
			endpos += circlepos;
			for (int i = 0; i < sensivity; i++)
			{
				if (!(i % 4))
				{
					glVertexAttrib3f(0, lightpos.x, lightpos.y, lightpos.z);
					glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);
				}

				glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
				glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

				startpos = endpos;
				endpos = glm::rotate((endpos - circlepos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
				endpos += circlepos;
			}
		}
		else if (lightcomponent->lightType == LightType::Directional)
		{
			dist = glm::distance(sceneCamera->position, lightpos) / 10.f;
			glm::vec3 startpos = lightpos + (lightActor->transformation.getWorldUpVector() * dist);
			glm::vec3 endpos = glm::rotate((startpos - lightpos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
			endpos += lightpos;
			glm::vec3 target;
			for (int i = 0; i < sensivity; i++)
			{
				if (!(i % 3))
				{
					glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);
					target = endpos + lightActor->transformation.getWorldForwardVector() *dist *3.f;
					glVertexAttrib3f(0, target.x, target.y, target.z);

				}

				glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
				glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

				startpos = endpos;
				endpos = glm::rotate((endpos - lightpos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
				endpos += lightpos;
			}
		}

	}

	
}

void SceneRenderer::renderLights()
{
	auto begin = GE_Engine->componentManager->begin<LightComponent>();
	auto end = GE_Engine->componentManager->end<LightComponent>();
	for (auto light = begin; light.operator!=(end); light.operator++())
	{
		Actor *lightActor = GE_Engine->actorManager->GetActor(light->owner);

		glm::vec3 worldpos = lightActor->transformation.getWorldPosition();
		spriteRenderer->DrawSprite(ConstantTextures::Textures::lightTexture, sceneCamera->worldToScreen(worldpos, sceneSize), { 30,30 });
	}

	
}

void SceneRenderer::renderSelectedLight()
{
	colorShader->setMat4("modelMatrix", glm::mat4(1));
	colorShader->setInt("hasBones", 0);
	colorShader->setVec3("color", glm::vec3(0.6f, 0.6f, 0.6f));

	glBegin(GL_LINES);
	
	if(Actor *lightActor = GE_Engine->actorManager->GetActor(selectedActor); lightActor && lightActor->GetComponent<LightComponent>())
	{
		
		LightComponent *lightcomponent = lightActor->GetComponent<LightComponent>();
		

		const glm::vec3 lightpos = lightActor->transformation.getWorldPosition();
		float dist = lightcomponent->distance;

		const int sensivity = 36;
		float constexpr degree = 360 / sensivity;

		if (lightcomponent->lightType == LightType::Point)
		{
			renderSphere(lightpos, dist, 18);
		}
		else if(lightcomponent->lightType == LightType::Spotlight)
		{
			glm::vec3 circlepos = lightpos + (lightActor->transformation.getWorldForwardVector() * dist);
			glm::vec3 startpos = circlepos + (lightActor->transformation.getWorldUpVector() * dist *std::tan(glm::radians(lightcomponent->angle)));
			glm::vec3 endpos = glm::rotate((startpos - circlepos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
			endpos += circlepos;
			for (int i = 0; i < sensivity; i++)
			{
				if (!(i % 4))
				{
					glVertexAttrib3f(0, lightpos.x, lightpos.y, lightpos.z);
					glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);
				}

				glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
				glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

				startpos = endpos;
				endpos = glm::rotate((endpos - circlepos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
				endpos += circlepos;
			}
		}
		else if (lightcomponent->lightType == LightType::Directional)
		{
			dist = glm::distance(sceneCamera->position, lightpos) / 10.f;
			glm::vec3 startpos = lightpos + (lightActor->transformation.getWorldUpVector() * dist);
			glm::vec3 endpos = glm::rotate((startpos - lightpos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
			endpos += lightpos;
			glm::vec3 target;
			for (int i = 0; i < sensivity; i++)
			{
				if (!(i % 3))
				{
					glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);
					target = endpos + lightActor->transformation.getWorldForwardVector() *dist *3.f;
					glVertexAttrib3f(0, target.x, target.y, target.z);
					
				}

				glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
				glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

				startpos = endpos;
				endpos = glm::rotate((endpos - lightpos), glm::radians(degree), lightActor->transformation.getWorldForwardVector());
				endpos += lightpos;
			}
		}

	}

	glEnd();
}

void SceneRenderer::renderSelectedCollider()
{
	Actor * selectedactor = GE_Engine->actorManager->GetActor(selectedActor);
	colorShader->setVec3("color", glm::vec3(0,0.9,1));
	colorShader->setMat4("modelMatrix", selectedactor->transformation.getWorldPose());
	glBegin(GL_LINES);
	
	if (SphereColliderComponent *spherecollider = selectedactor->GetComponent<SphereColliderComponent>(); spherecollider)
	{
		renderSphere({ 0,0,0 }, spherecollider->geometry.radius);
	}
	if (CubeColliderComponent *cubecollider = selectedactor->GetComponent<CubeColliderComponent>(); cubecollider)
	{
		renderCube({ 0,0,0 }, cubecollider->geometry.halfExtents.x, cubecollider->geometry.halfExtents.y, cubecollider->geometry.halfExtents.z);
	}
	glEnd();

	if (CapsuleColliderComponent *capsulecollider = selectedactor->GetComponent<CapsuleColliderComponent>(); capsulecollider)
	{
		glm::vec3 t(0,0,1);
		
	/*	if (capsulecollider->upp == 0)
			t = { 0,0,1 };
		else if (capsulecollider->upp == 1)
		{
			t = { 0,1,0 };
			colorShader->setMat4("modelMatrix", glm::rotate(glm::mat4(1), glm::radians(90.f), t));
		}
		else if (capsulecollider->upp == 2)
		{
			t = { 1,0,0 };
			colorShader->setMat4("modelMatrix", glm::rotate(glm::mat4(1), glm::radians(90.f), t));
		}*/
		glBegin(GL_LINES);
		renderCapsule(capsulecollider->upp, capsulecollider->geometry.radius, capsulecollider->geometry.halfHeight);
		glEnd();
		colorShader->setMat4("modelMatrix",glm::mat4(1));
	}

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
	
	if (selectedactor)
	{
		renderSelectedCamera();
		renderSelectedCollider();
		renderSelectedLight(); //first light because color shader is still identity 
		RenderOutlined(selectedactor);
		
	}

	spriteShader->Use();
	spriteShader->setMat4("projectionMatrix", glm::ortho(0.0f, sceneSize.x, 0.0f, sceneSize.y, -1.0f, 1.0f));
	renderLights();

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

		sceneTool->Render(&selectedactor->transformation, glm::distance(sceneCamera->position, selectedactor->transformation.getWorldPosition()) / 10, sceneCamera->position);

		glEnable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
inline void SceneRenderer::RenderOutlined(Actor * o)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	normalShader->Use();
	normalShader->setMat4("modelMatrix", o->transformation.getWorldMatrix());
	RenderAnActor(o, normalShader);

	colorShader->Use();
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	colorShader->setVec3("color", glm::vec3(1, 1, 0));

	
	colorShader->setMat4("modelMatrix", glm::scale(o->transformation.getWorldMatrix(), { 1.1, 1.1, 1.1 }));
	RenderAnActor(o, colorShader);


	glDisable(GL_STENCIL_TEST);
}

void SceneRenderer::renderModelsColored()
{

	colorShader->Use();
	colorShader->setInt("hasBones", 0);
	auto mend = GE_Engine->componentManager->end<ModelComponent>();
	for (auto model = GE_Engine->componentManager->begin<ModelComponent>(); model.operator!=(mend); model.operator++())
	{
		if (!model->getModel())
			continue;
		Actor *actor = GE_Engine->actorManager->GetActor(model->owner);
		ModelComponent *mcmp = actor->GetComponent<ModelComponent>();
		ActorID test = model->owner;
		float r = (test.index & 0x000000FF) >> 0;
		float g = (test.index & 0x0000FF00) >> 8;
		float b = (test.index & 0x00FF0000) >> 16;
		colorShader->setVec3("color", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
		colorShader->setMat4("modelMatrix", actor->transformation.getWorldMatrix());
		for (int i = 0; i < model->getModel()->numOfMeshes; i++)
		{
			model->getModel()->meshes[i]->bind();
			model->getModel()->meshes[i]->render();
			model->getModel()->meshes[i]->unbind();
		}
	}

	colorShader->setInt("hasBones", 1);
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
					model->getModel()->meshes[i]->bind();
					ActorID test = model->owner;
					float r = (test.index & 0x000000FF) >> 0;
					float g = (test.index & 0x0000FF00) >> 8;
					float b = (test.index & 0x00FF0000) >> 16;
					colorShader->setVec3("color", glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));

					std::vector<glm::mat4> matrixBuffer;
					int j = 0;
					for (auto x : model->effectlist[i])
					{
						matrixBuffer.push_back((GE_Engine->actorManager->GetActor(x)->transformation.getWorldMatrix()) * glm::transpose(((SkinnedMesh*)model->getModel()->meshes[i])->bones[j].second));
						j++;
					}
					colorShader->setMat4Array("bones", matrixBuffer.size(), matrixBuffer.data()[0]);

					model->getModel()->meshes[i]->render();
					model->getModel()->meshes[i]->unbind();
				}
	
		}
	}
}

ActorID SceneRenderer::RenderForObjectPicker(GLint x, GLint y)
{
	if(x < 0 || y < 0)
		return ActorID::INVALID_HANDLE;
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

	sceneTool->Render(&selectedactor->transformation, glm::distance(sceneCamera->position, selectedactor->transformation.getWorldPosition()) / 10, sceneCamera->position);

	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return sceneTool->processTool(test, &selectedactor->transformation);
}

void SceneRenderer::focusActor(ActorID actorid)
{
	if (actorid == ActorID::INVALID_HANDLE)
		return;
	//@TODO CALC MÝN MAX FOR EACH MESH AND CHECK MAYBE THERE IS NO MODEL
	Actor * actor = GE_Engine->actorManager->GetActor(actorid);
	if (!actor->GetComponent<ModelComponent>())
		return;

	ModelComponent *mcmp = actor->GetComponent<ModelComponent>();
	auto x = mcmp->getModel()->meshes[0]->bounds;

	glm::vec4 minxyz(x.minx, x.miny, x.minz, 1.0);
	glm::vec4 maxxyz(x.maxx, x.maxy, x.maxz, 1.0);

  	minxyz = actor->transformation.getWorldMatrix() * minxyz;
	maxxyz = actor->transformation.getWorldMatrix() *maxxyz;
	
	minxyz = minxyz * (float)(1.0 / minxyz.w);
	maxxyz = maxxyz * (float)(1.0 / maxxyz.w);

	float dis = glm::distance(minxyz, maxxyz);


	sceneCamera->pitch = -25;
	sceneCamera->yaw = 145;
	glm::vec3 look(1, -1, 1);
	glm::normalize(look);
	look *=  dis;

	sceneCamera->position = actor->transformation.getWorldPosition() + look;
	sceneCamera->UpdateCameraVectors();
	
}

void SceneRenderer::renderSphere(const glm::vec3 &pos, float radius, int sensivity)
{
	const float degree = 360 / sensivity;

	glm::vec3 startpos = pos + (glm::vec3(1, 0, 0) *radius);
	glm::vec3 endpos = glm::rotate((startpos - pos), glm::radians(degree), glm::vec3(0, 1, 0));
	endpos += pos;

	for (int i = 0; i < sensivity; i++)
	{
		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - pos), glm::radians(degree), glm::vec3(0, 1, 0));
		endpos += pos;
	}

	startpos = pos + (glm::vec3(1, 0, 0) *radius);
	endpos = glm::rotate((startpos - pos), glm::radians(degree), glm::vec3(0, 0, 1));
	endpos += pos;

	for (int i = 0; i < sensivity; i++)
	{
		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - pos), glm::radians(degree), glm::vec3(0, 0, 1));
		endpos += pos;
	}

	startpos = pos + (glm::vec3(0, 1, 0) *radius);
	endpos = glm::rotate((startpos - pos), glm::radians(degree), glm::vec3(1, 0, 0));
	endpos += pos;

	for (int i = 0; i < sensivity; i++)
	{
		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - pos), glm::radians(degree), glm::vec3(1, 0, 0));
		endpos += pos;
	}

	startpos = pos + (sceneCamera->rightVector * radius);
	endpos = glm::rotate((startpos - pos), glm::radians(degree), sceneCamera->frontVector);
	endpos += pos;

	for (int i = 0; i < sensivity; i++)
	{
		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - pos), glm::radians(degree), sceneCamera->frontVector);
		endpos += pos;
	}
}

void SceneRenderer::renderCapsule(int up, float radius, float halfheight, int sensivity)
{
	const float degree = 360 / sensivity;

	glm::vec3 startup[4],startdown[4];

	/*
	if (upp == 0)
			up = { 0,0,1 };
		else if (upp == 1)
			up = { 1,0,0 };
		else
			up = { 0,1,0 };
	*/
	glm::vec3 twup,twright,twfront;
	switch (up)
	{
	case 0:
		twright = { 1,0,0 };
		twup = { 0,0,1 };
		twfront = { 0,1,0 };
		break;
	case 1:
		twright = { 0,0,1 };
		twfront = { 0,1,0 };
		twup = { 1,0,0 };
		break;
	case 2:
		twright = { 1,0,0 };
		twfront = { 0,0,1 };
		twup = { 0,1,0 };
		break;
	}

	glm::vec3 circlepos = /*pos + */(twup * halfheight);
	glm::vec3 startpos = circlepos + (twright *radius);
	glm::vec3 endpos = glm::rotate((startpos - circlepos), glm::radians(degree), twup);
	endpos += circlepos;
	for (int i = 0; i < sensivity; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twup);
		endpos += circlepos;
	}
	startpos = circlepos + (twup *radius);
	endpos = glm::rotate((startpos - circlepos), glm::radians(-sensivity/4*degree), twright);
	endpos += circlepos;
	startpos = glm::rotate((endpos - circlepos), glm::radians(-degree), twright);
	startpos += circlepos;
	startup[0] = startpos;

	for (int i = 0; i < sensivity/2; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twright);
		endpos += circlepos;
	}
	startup[1] = startpos;

	startpos = circlepos + (twup *radius);
	endpos = glm::rotate((startpos - circlepos), glm::radians(-sensivity / 4 * degree), twfront);
	endpos += circlepos;
	startpos = glm::rotate((endpos - circlepos), glm::radians(-degree), twfront);
	startpos += circlepos;
	startup[2] = startpos;

	for (int i = 0; i < sensivity / 2; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twfront);
		endpos += circlepos;
	}
	startup[3] = startpos;



	circlepos = /*pos*/ - (twup * halfheight);
	startpos = circlepos + (twright *radius);
	endpos = glm::rotate((startpos - circlepos), glm::radians(degree), twup);
	endpos += circlepos;
	for (int i = 0; i < sensivity; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twup);
		endpos += circlepos;
	}

	/**/
	startpos = circlepos - (twup *radius);
	endpos = glm::rotate((startpos - circlepos), glm::radians(-sensivity / 4 * degree), twright);
	endpos += circlepos;
	startpos = glm::rotate((endpos - circlepos), glm::radians(-degree), twright);
	startpos += circlepos;
	startdown[0] = startpos;

	for (int i = 0; i < sensivity / 2; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twright);
		endpos += circlepos;
	}
	startdown[1] = startpos;

	startpos = circlepos - (twup *radius);
	endpos = glm::rotate((startpos - circlepos), glm::radians(-sensivity / 4 * degree), twfront);
	endpos += circlepos;
	startpos = glm::rotate((endpos - circlepos), glm::radians(-degree), twfront);
	startpos += circlepos;
	startdown[2] = startpos;

	for (int i = 0; i < sensivity / 2; i++)
	{

		glVertexAttrib3f(0, startpos.x, startpos.y, startpos.z);
		glVertexAttrib3f(0, endpos.x, endpos.y, endpos.z);

		startpos = endpos;
		endpos = glm::rotate((endpos - circlepos), glm::radians(degree), twfront);
		endpos += circlepos;
	}
	startdown[3] = startpos;


		glVertexAttrib3f(0, startup[0].x, startup[0].y, startup[0].z);
		glVertexAttrib3f(0, startdown[1].x, startdown[1].y, startdown[1].z);
		glVertexAttrib3f(0, startup[1].x, startup[1].y, startup[1].z);
		glVertexAttrib3f(0, startdown[0].x, startdown[0].y, startdown[0].z);

		glVertexAttrib3f(0, startup[2].x, startup[2].y, startup[2].z);
		glVertexAttrib3f(0, startdown[3].x, startdown[3].y, startdown[3].z);
		glVertexAttrib3f(0, startup[3].x, startup[3].y, startup[3].z);
		glVertexAttrib3f(0, startdown[2].x, startdown[2].y, startdown[2].z);

	

}

void SceneRenderer::renderCube(const glm::vec3 & pos, float x, float y, float z)
{
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 + z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 + z);
							   	  
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 - z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 - z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 - z);
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 - z);
					
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 + z);
	glVertexAttrib3f(0, 0 - x, 0 - y, 0 - z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 - x, 0 + y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 - y, 0 - z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 + z);
	glVertexAttrib3f(0, 0 + x, 0 + y, 0 - z);

}


