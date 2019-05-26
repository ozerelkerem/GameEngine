#pragma once
#include <engine/Renderer.h>
class GameRenderer : public Renderer
{


public:
	GameRenderer(GameBase *);
	~GameRenderer();


	void render();
};

