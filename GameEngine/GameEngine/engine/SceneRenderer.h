#pragma once

#include <stdlib.h>
#include <iostream>

#include "Grid.h"
#include "SceneCamera.h"
#include "Shader.h"
#include "Scene.h"
#include "Model.h"

#include "../ogl/glm/glm.hpp"

#define sceneMaxWidth 2000
#define sceneMaxHeight 2000

class Model;


class SceneRenderer
{
private:
	unsigned int textureColorbuffer;
	unsigned int framebuffer;

	Grid *grid;

	Model *m1;

	glm::vec2 size;
public:
	glm::vec3 backgroundColor;
	SceneCamera *sceneCamera;
	Scene * scene;

	Shader *shader;
	Shader *objectPickShader;

	Object *selectedObject; // for object properties
	Object *hoveredObject; // for drag and drop at hierarchy

	SceneRenderer();
	~SceneRenderer();

	void GenerateBuffers();

	void RenderPoint(GLint x, GLint y);
	void RenderForObjectPicker(GLint x, GLint y);
	void Render();
	void Update(glm::vec2 size);

	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }
	inline Grid *GetGrid() { return grid; }
};

