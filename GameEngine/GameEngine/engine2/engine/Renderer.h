#pragma once
#include <engine/Shader.h>
#include <engine/GameBase.h>

extern Shader *normalShader;
extern Shader *colorShader;

class Actor;

class Renderer
{
public:
	Renderer(GameBase *gb);
	~Renderer();

	void RenderAnActor(Model * model);
	void RenderAnActor(Actor *);

	/*Shaders*/

	GameBase *gamebase;

	virtual void render() = 0;
	void renderModels();
	void prepareLights();

};

