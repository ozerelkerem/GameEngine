#pragma once

#include <stdlib.h>
#include <iostream>

#include "Grid.h"
#include "SceneCamera.h"
#include "Shader.h"
#include "Scene.h"
#include "Model.h"
#include "SceneTools.h"
#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtx/matrix_decompose.hpp"

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

	SceneTools *sceneTool;


	Object *selectedObject; // for object properties
	Object *hoveredObject; // for drag and drop at hierarchy

	SceneRenderer();
	~SceneRenderer();

	void GenerateBuffers();

	inline void RenderOutlined(Object *);
	Object * RenderForObjectPicker(GLint x, GLint y);
	bool RenderForObjectTools(GLint x, GLint y);
	
	void Render();
	void Update(glm::vec2 size);

	
	inline unsigned int GetTextureColorBuffer() { return textureColorbuffer; }
	inline Grid *GetGrid() { return grid; }
};

