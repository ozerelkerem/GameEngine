#pragma once
#include<Api.h>
#include <engine/Shader.h>
#include <engine/GameBase.h>

#define sceneMaxWidth 2000
#define sceneMaxHeight 2000

class SceneCamera;
class Actor;

class Renderer
{
private:
	unsigned int textureColorbuffer;
protected:
	unsigned int framebuffer;

public:
	Renderer(GameBase *gb);
	~Renderer();

	void RenderAnActor(Model * model);
	void RenderAnActor(Actor *, Shader *);

	/*Shaders*/

	GameBase *gamebase;

	virtual void render() = 0;

	void GenerateBuffers();

	void renderModels();

	void prepareLights();

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }

};

