#pragma once

#include <stdlib.h>

#include "Grid.h"
#include "SceneCamera.h"
#include "Shader.h"
#include "Scene.h"

#include "../ogl/glm/glm.hpp"

#define sceneMaxWidth 2000
#define sceneMaxHeight 2000

class SceneRenderer
{
private:
	unsigned int textureColorbuffer;
	unsigned int framebuffer;

	Grid *grid;
	Shader *shader;

	glm::vec2 size;
public:
	glm::vec3 backgroundColor;
	SceneCamera *sceneCamera;
	Scene * scene;

	Object *selectedObject; // for object properties
	Object *hoveredObject; // for drag and drop at hierarchy

	SceneRenderer();
	~SceneRenderer();

	void GenerateBuffers();

	void Render();
	void Update(glm::vec2 size);

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }
	inline Grid *GetGrid() { return grid; }
};

