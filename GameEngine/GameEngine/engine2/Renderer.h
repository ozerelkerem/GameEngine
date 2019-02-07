#pragma once
#include <Shader.h>
#include <GameBase.h>
class Renderer
{
public:
	Renderer(GameBase *gb);
	~Renderer();

	/*Shaders*/
	Shader *normalShader;

	GameBase *gamebase;


	virtual void render() = 0;
	void renderModels();

};

