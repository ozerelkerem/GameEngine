#pragma once

#include <Scene.h>
#include <Shader.h>

class GameBase
{
public:
	GameBase(Scene *);
	~GameBase();

	Scene *currentScene;

};