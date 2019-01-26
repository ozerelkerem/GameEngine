#include "SceneRenderer.h"

#include "../ogl/GL/glew.h"

SceneRenderer::SceneRenderer()
{
	scene = new Scene();
	sceneCamera = new SceneCamera(glm::vec3(20, -50, 20), 90, 0);
	shader = new Shader("engine/shaders/mainShadervs.glsl", "engine/shaders/mainShaderfs.glsl");
	grid = new Grid(16, 1, shader->getProgramID());

	backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);

	shader->Use();
	m1 = ModelFunctions::CreateCube(scene);

	selectedObject = NULL;
	hoveredObject = NULL;

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

void SceneRenderer::Render()
{
	shader->Use();

	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m1->meshes[0]->Render();

	grid->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::Update(glm::vec2 size)
{
	this->size = size;

	shader->setMat4("viewMatrix", sceneCamera->getViewMatrix());
	shader->setMat4("projectionMatrix", glm::perspective(30.0f, (float)(size.x / size.y), 0.1f, 1000.0f));
}