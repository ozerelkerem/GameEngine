#pragma once
#include <Shader.h>
#include <GameBase.h>

extern Shader *normalShader;
extern Shader *colorShader;
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

};

