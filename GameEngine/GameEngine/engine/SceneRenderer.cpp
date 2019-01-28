#include "SceneRenderer.h"

#include "../ogl/GL/glew.h"

Shader * colorShader;

SceneRenderer::SceneRenderer()
{
	sceneTool = new SceneTools();
	scene = new Scene();
	sceneCamera = new SceneCamera(glm::vec3(20, -50, 20), 90, 0);
	shader = new Shader("engine/shaders/mainShadervs.glsl", "engine/shaders/mainShaderfs.glsl");

	objectPickShader = new Shader("engine/shaders/objectPickervs.glsl", "engine/shaders/objectPickerfs.glsl");
	colorShader = objectPickShader;
	grid = new Grid(16, 1, shader->getProgramID());

	backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);

	m1 = ModelFunctions::CreateCube(scene);

	selectedObject = NULL;
	hoveredObject = NULL;
	
	//glEnable(GL_CULL_FACE);
	
	GenerateBuffers();
}

SceneRenderer::~SceneRenderer()
{
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

inline void SceneRenderer::RenderOutlined(Object * o)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	static_cast<Model *>(o)->Render(shader);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	objectPickShader->Use();
	objectPickShader->setVec3("color", glm::vec3(1,1,0));
	o->transform->modelMatrix = glm::scale(o->transform->modelMatrix, { 1.1,1.1,1.1 });
	static_cast<Model *>(o)->Render(objectPickShader);
	o->transform->calcModelMatrix();

	glDisable(GL_STENCIL_TEST);
}

Object * SceneRenderer::RenderForObjectPicker(GLint x, GLint y)
{
	objectPickShader->Use();
	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); 
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectPickShader->setVec3("color", glm::vec3(m1->id/255.f, 0, 0));
	m1->Render(objectPickShader);
	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (test[0] == 0)
		return NULL;
	else
		return scene->getObjectByID((int)(test[0] * 255));
	
}

bool SceneRenderer::RenderForObjectTools(GLint x, GLint y)
{
	if (!selectedObject)
		return false;
	colorShader->Use();
	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneTool->Render(selectedObject->transform, glm::distance(sceneCamera->position, selectedObject->transform->position)/ 10);


	GLfloat test[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &test);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 
	Update(size);

	return sceneTool->processTool(test, selectedObject->transform);
	


}



void SceneRenderer::Render()
{

	shader->Use();
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (selectedObject != m1)
		m1->Render(shader);
	
	


	shader->Use();
	shader->setMat4("modelMatrix", glm::mat4(1));
	grid->Draw();

	if (selectedObject)
	{
		RenderOutlined(selectedObject);
		glDisable(GL_DEPTH_TEST);

		sceneTool->Render(selectedObject->transform, glm::distance(sceneCamera->position, selectedObject->transform->position) / 10);

		glEnable(GL_DEPTH_TEST);
	}
		

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}



void SceneRenderer::Update(glm::vec2 size)
{
	this->size = size;
	
	sceneCamera->projectionMatrix = glm::perspective(30.0f, (float)(size.x / size.y), 0.1f, 1000.0f);

	shader->Use();
	shader->setMat4("viewMatrix", sceneCamera->viewMatrix);
	shader->setMat4("projectionMatrix", sceneCamera->projectionMatrix);

	objectPickShader->Use();
	objectPickShader->setMat4("viewMatrix", sceneCamera->getViewMatrix());
	objectPickShader->setMat4("projectionMatrix", sceneCamera->projectionMatrix);

}

glm::vec3 SceneRenderer::screenToWorld(int x, int y)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	float depth;



	glm::vec2 screencords = sceneCamera->worldToScreen(selectedObject->transform->position, size);
	
	glReadPixels(screencords.x, screencords.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	x = glm::clamp((float)x,40.f,size.x-50);
	glm::vec3 cor = glm::unProject(glm::vec3(x, y, depth), sceneCamera->getViewMatrix(), glm::perspective(30.0f, (float)(size.x / size.y), 0.1f, 1000.0f), glm::vec4(0, 0, size.x, size.y));
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return cor;
}


