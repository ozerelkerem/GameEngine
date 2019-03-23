#pragma once
#include<Api.h>
#include <engine/Shader.h>
#include <engine/GameBase.h>


class SceneCamera;
class Actor;

class Renderer
{
public:
	Renderer(GameBase *gb);
	~Renderer();

	void RenderAnActor(Model * model);
	void RenderAnActor(Actor *, Shader *);

	/*Shaders*/

	GameBase *gamebase;

	virtual void render() = 0;

	void renderModels();
	void renderLights(SceneCamera *camera);

	void prepareLights();

};

