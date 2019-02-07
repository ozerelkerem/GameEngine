#pragma once
#include <Renderer.h>
#include <SceneCamera.h>
#define sceneMaxWidth 2000
#define sceneMaxHeight 2000

class SceneRenderer :
	public Renderer
{
public:
	SceneRenderer(GameBase *b);
	~SceneRenderer();

	glm::vec2 sceneSize;

	SceneCamera *sceneCamera;

	void render();

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }

private:
	unsigned int textureColorbuffer;
	unsigned int framebuffer;
	void GenerateBuffers();



};

