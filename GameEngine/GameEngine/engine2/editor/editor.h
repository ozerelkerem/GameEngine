#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imconfig.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <editor/SceneRenderer.h>
#include <editor/ProjectManager.h>

class Editor
{
public:
	Editor(GLFWwindow *);
	~Editor();

	GLFWwindow *window;
	ImVec2 viewportSize = { 1280,720 };
	double prevMousePosition[2];

	bool travelMode = false, travelMode2 = false, toolMode = false;

	SceneRenderer * sceneRenderer;

	ProjectManager *projectManager;


	GameBase *gameBase;

	void Render();
	void DrawHierarchy(Actor *);

	void ShowComponentList();
	void ObjectProperties();


	void handle_dropped_file(const char *path);
	


};