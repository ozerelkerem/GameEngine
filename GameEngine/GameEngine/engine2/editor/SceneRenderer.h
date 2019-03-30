#pragma once
#include<Api.h>

#include <engine/Renderer.h>
#include <engine/Actor.h>
#include <engine/ActorManager.h>
#include<engine/SpriteRenderer.h>
#include <editor/SceneCamera.h>
#include <editor/Grid.h>
#include <editor/SceneTools.h>
#include<engine/components/colliders/SphereColliderComponent.h>
#include<engine/components/colliders/CapsuleColliderComponent.h>
#include<engine/components/colliders/CubeColliderComponent.h>

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
	SpriteRenderer *spriteRenderer;
	SceneCamera *sceneCamera;
	Grid *grid;
	glm::vec3 backgroundColor;

	SceneTools *sceneTool;

	ActorID selectedActor;
	ActorID hoveredActor;

	void renderLights();
	void renderSelectedLight();
	void renderSelectedCollider();
	void render();

	void renderModelsColored();
	void RenderOutlined(Actor * o);
	ActorID RenderForObjectPicker(GLint x, GLint y);
	bool RenderForObjectTools(GLint x, GLint y);

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }

	void focusActor(ActorID);

	void renderSphere(const glm::vec3 &pos, float radius, int sensivity=36);
	void renderCapsule(const glm::vec3 &pos, float radius, float halfheight,int sensivity = 36);
	void renderCube(const glm::vec3 &pos, float x, float y, float z);

};

