#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stdio.h>
#include <iostream>

#include "ogl/GL/glew.h" 
#include "ogl/GLFW/glfw3.h"
#include "ogl/glm/glm.hpp"

#include "engine/Scene.h"
#include "engine/Shader.h"
#include "engine/SceneCamera.h"
#include "engine/Grid.h"
#include "engine/SceneRenderer.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

ImVec2 size = { 1280,720 };
SceneRenderer *sceneRenderer;

void drawHiearchy(Object*);


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

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	glfwSetWindowSizeCallback(window, resize_callback);

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

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	sceneRenderer = new SceneRenderer();

	double prevMousePosition[2];
	bool travelMode = false, travelMode2 = false;
	

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		sceneRenderer->Update(glm::vec2(size.x, size.y));
		sceneRenderer->Render();


		{
			static bool opt_fullscreen_persistant = true;
			static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
			bool opt_fullscreen = opt_fullscreen_persistant;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", NULL, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();

			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
					if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
					if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruDockspace) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruDockspace;
					ImGui::Separator();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			ImGui::End();



			ImGui::Begin("Scene", NULL);
			{
				size = ImGui::GetWindowSize();
				size.y -= 35;
				ImGui::Image((void*)sceneRenderer->GetTextureColorBuffer(), size, { 0,0 }, { size.x / sceneMaxWidth, size.y / sceneMaxHeight });


				//travel mode1 rotating camera if travelmode1 works, travelmode2 does not
				{
					if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
					{
						glfwGetCursorPos(window, &prevMousePosition[0], &prevMousePosition[1]);
						travelMode = true;
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					if (ImGui::IsMouseReleased(1))
					{
						travelMode = false;
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
					if (travelMode && (io.MouseDownDuration[1] > 0))
					{
						double midx, midy;
						midx = (int)(ImGui::GetWindowPos().x + size.x / 2);
						midy = (int)(ImGui::GetWindowPos().y + size.y / 2);

						double x, y;

						ImGui::GetWindowDrawList()->AddCircle({ (float)midx,(float)midy }, 10.f, IM_COL32(255, 0, 0, 255), 12, 2.f);


						glfwGetCursorPos(window, &x, &y);


						sceneRenderer->sceneCamera->RotatePitch(-(y - prevMousePosition[1])*0.01);
						sceneRenderer->sceneCamera->RotateYaw(-(x - prevMousePosition[0])*0.01);

						prevMousePosition[0] = midx;
						prevMousePosition[1] = midy;

						glfwSetCursorPos(window, midx, midy);
					}
				}
				

				//travel mode2 camera up down
				{
					if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(ImGuiMouse_Middle_) && !travelMode)
					{
						glfwGetCursorPos(window, &prevMousePosition[0], &prevMousePosition[1]);
						travelMode2 = true;
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					}
					if (ImGui::IsMouseReleased(1) && !travelMode)
					{
						travelMode2 = false;
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}

					if (travelMode2 && (io.MouseDownDuration[ImGuiMouse_Middle_] > 0) && !travelMode)
					{
						double midx, midy;
						midx = (int)(ImGui::GetWindowPos().x + size.x / 2);
						midy = (int)(ImGui::GetWindowPos().y + size.y / 2);

						double x, y;

						ImGui::GetWindowDrawList()->AddCircle({ (float)midx,(float)midy }, 10.f, IM_COL32(255, 0, 0, 255), 12, 2.f);


						glfwGetCursorPos(window, &x, &y);


						sceneRenderer->sceneCamera->MoveUp(-(y - prevMousePosition[1])*0.01);

						prevMousePosition[0] = midx;
						prevMousePosition[1] = midy;

						glfwSetCursorPos(window, midx, midy);
					}
				}
				

				
				if (ImGui::IsMouseHoveringWindow() && (io.MouseDownDuration[1] > 0))
				{
					if ((io.KeysDownDuration[ImGuiKey_W_] > 0))
						sceneRenderer->sceneCamera->MoveForward();
					else if ((io.KeysDownDuration[ImGuiKey_S_] > 0))
						sceneRenderer->sceneCamera->MoveBackward();

					if ((io.KeysDownDuration[ImGuiKey_A_] > 0))
						sceneRenderer->sceneCamera->MoveLeft();
					else if ((io.KeysDownDuration[ImGuiKey_D_] > 0))
						sceneRenderer->sceneCamera->MoveRight();
				}

				if (ImGui::IsMouseHoveringWindow() && (io.MouseWheel != 0))
				{
					if (io.MouseDownDuration[1] > 0)
					{
						sceneRenderer->sceneCamera->cameraSpeed += io.MouseWheel * 0.01;
						if (sceneRenderer->sceneCamera->cameraSpeed <= 0.01)
							sceneRenderer->sceneCamera->cameraSpeed = 0.01;
					}

					else
						if (io.MouseWheel > 0)
							sceneRenderer->sceneCamera->MoveForward();
						else
							sceneRenderer->sceneCamera->MoveBackward();
				}





			}
			ImGui::End();

			ImGui::Begin("Game", NULL);
			{






			}
			ImGui::End();

			ImGui::Begin("Scene Properties", NULL);
			{
				if (ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragInt("Line Count", &(sceneRenderer->GetGrid()->lineCount), 1.f, 2.f, 1000.f);
					ImGui::DragFloat("Space Lenght", &(sceneRenderer->GetGrid()->spaceLength), 1.0f, 0.01f, 1000.f);
					ImGui::ColorEdit3("Grid Color", (sceneRenderer->GetGrid()->gridColor));
				}
				if (ImGui::CollapsingHeader("Render Settings", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::ColorEdit3("Background Color", &(sceneRenderer->backgroundColor[0]));
				}
				if (ImGui::CollapsingHeader("World Settings", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::DragFloat("Camera Speed", &(sceneRenderer->sceneCamera->cameraSpeed), 0.1f, 0.01f, 100.f);
					ImGui::DragFloat("Camera Rotate Speed", &(sceneRenderer->sceneCamera->mouseSensivity), 0.1f, 0.01f, 100.f);
				}
			}
			ImGui::End();

			ImGui::Begin("Object Properties", NULL);
			{
				if (!sceneRenderer->selectedObject)
					ImGui::Text("Please Select a Object");
				else
				{

					if (ImGui::CollapsingHeader("Transformations", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::DragFloat3("Position", &sceneRenderer->selectedObject->transform->position[0], 1.f, -30000.f, 30000.f);
					}
				}
			}
			ImGui::End();

			ImGui::Begin("Project Explorer", NULL);
			ImGui::End();

			ImGui::Begin("Hierarchy", NULL);
			{
				drawHiearchy(sceneRenderer->scene->rootObject);	
			}
			ImGui::End();
			ImGui::Begin("Create Object", NULL);
			ImGui::End();
		}

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

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void drawHiearchy(Object * root)
{
	int n;
	ImGuiTreeNodeFlags src_flags = 0;
	if (root->numOfChilds == 0)
		src_flags = ImGuiTreeNodeFlags_Leaf;
	else
		src_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow;
	if (sceneRenderer->selectedObject == root)
		src_flags |= ImGuiTreeNodeFlags_Selected;
	

	bool node_open = ImGui::TreeNodeEx((void *) root,src_flags,root->name.c_str());
	
	//right click popup
	{
		ImGui::PushID(root->name.c_str());
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete") && root->name != "root")
			{
				root->RemoveObject();
				sceneRenderer->selectedObject = NULL;
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
	

	{
		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
		//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
		//drag and drop 
		{
			if (root->name != "root" && ImGui::BeginDragDropSource(src_flags))
			{
				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				{
					ImGui::Text("%s", root->name.c_str());
					sceneRenderer->hoveredObject = root;
				}

				ImGui::SetDragDropPayload("DND_DEMO_NAME", &n, sizeof(int));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME"))
				{
					if (sceneRenderer->hoveredObject)
						sceneRenderer->hoveredObject->AddParent(root);
					sceneRenderer->hoveredObject = NULL;
				}
				ImGui::EndDragDropTarget();
			}
		}
		
		if (ImGui::IsItemClicked() && sceneRenderer->scene->rootObject != root)
		{
			sceneRenderer->selectedObject = root;
		}
		
		if (node_open)
		{
			for (int i = 0; i < root->numOfChilds; i++)
				drawHiearchy(root->childs[i]);
			ImGui::TreePop();
		}
	}
}


