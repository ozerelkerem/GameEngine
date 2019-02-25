#pragma once
#include <Renderer.h>

#include <SceneCamera.h>
#include <Grid.h>
#include <Actor.h>
#include <SceneTools.h>

#define sceneMaxWidth 2000
#define sceneMaxHeight 2000

class SceneTools;

class SceneRenderer :
	public Renderer
{
private:
	unsigned int textureColorbuffer;
	unsigned int framebuffer;
	void GenerateBuffers();

public:
	SceneRenderer(GameBase *b);
	~SceneRenderer();

	glm::vec2 sceneSize;

	SceneCamera *sceneCamera;
	Grid *grid;
	glm::vec3 backgroundColor;

	SceneTools *sceneTool;

	Actor *selectedActor;
	Actor *hoveredActor;

	void render();

	void RenderOutlined(Actor * o);
	Actor * RenderForObjectPicker(GLint x, GLint y);
	bool RenderForObjectTools(GLint x, GLint y);

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }

};

