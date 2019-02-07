#pragma once

#include <Scene.h>
class GameBase
{
public:
	GameBase(Scene *);
	~GameBase();

	Scene *currentScene;

};

