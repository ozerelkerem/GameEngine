#pragma once

#include <engine/Scene.h>
#include <engine/Shader.h>

class GameBase
{
public:
	GameBase(Scene *);
	~GameBase();

	Scene *currentScene;
};