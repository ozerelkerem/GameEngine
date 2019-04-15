




#include<Api.h>

#include<editor/projectselect.h>
#include <editor/editor.h>
#include<engine/InputManager.h>
#include "ogl/GL/glew.h" 
#include "ogl/GLFW/glfw3.h"
#include "ogl/glm/glm.hpp"

#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include "ogl/GLFW/glfw3native.h"


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

ImVec2 viewportSize = { 1280,720 };

Editor *editor;
projectselect * projectSelectWindow;




void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	int i;
	for (i = 0; i < count; i++)
		editor->handle_dropped_file(paths[i]);
}


void resize_callback(GLFWwindow * window, int width, int height)
{
	//size.x = width;
	//size.y = height;
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	GLFWwindow* window2 = glfwCreateWindow(1280, 720, "Select Project", NULL, NULL);
	if (window2 == NULL)
		return 1;

	glfwMakeContextCurrent(window2);
	glfwSwapInterval(1); // Enable vsync

	glfwSetWindowSizeCallback(window2, resize_callback);
	glfwSetDropCallback(window2, drop_callback);

	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window2, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	projectSelectWindow = new projectselect();

	// Proje seçme
	while (!glfwWindowShouldClose(window2) && !projectSelectWindow->isSelected())
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Rendering
		projectSelectWindow->Render();
		ImGui::Render();
		

		int display_w, display_h;
		glfwGetFramebufferSize(window2, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window2);


	}
	if (!projectSelectWindow->projectManager)
		return 0;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwDestroyWindow(window2);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Game Engine", NULL, NULL);
	if (window == NULL)
		return 1;

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetDropCallback(window, drop_callback);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	Initialize();
	editor = new Editor(window, projectSelectWindow->projectManager);
	GE_Engine->projectManager = projectSelectWindow->projectManager;
	glfwSetKeyCallback(window2, InputManager::key_Callback);
	glfwSetMouseButtonCallback(window2, InputManager::mousebutton_Callback);
	static double x, y;
	glfwGetCursorPos(window2, &x, &y);
	InputManager::mouseposition_Callback(window2, x, y);
	glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/////
		GE_Engine->Update();
		editor->Render();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
		
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	
	glfwTerminate();

	return 0;
}

