#include "Editor.h"



Editor::Editor(GLFWwindow *window)
{
	
	this->window = window;
	scene = new Scene("Taso");
	gameBase = new GameBase(scene);
	sceneRenderer = new SceneRenderer(gameBase);
}


Editor::~Editor()
{
}

void Editor::Render()
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
		viewportSize = ImGui::GetWindowSize();
		viewportSize.y -= 35;

		//sceneRenderer->Update(glm::vec2(viewportSize.x, viewportSize.y));

		sceneRenderer->sceneSize.x = viewportSize.x;
		sceneRenderer->sceneSize.y = viewportSize.y;

		ImGui::SetCursorPosY(21);
		ImGui::SetCursorPosX(0);

		int x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x;
		int y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 20;

		if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			toolMode = travelMode = travelMode2 = false;
			sceneRenderer->selectedActor = NULL;
		}

		//toolmode
		{
			if (ImGui::IsKeyDown(GLFW_KEY_S) && !travelMode)
				sceneRenderer->sceneTool->mode = SCALE;

			if (ImGui::IsKeyDown(GLFW_KEY_R) && !travelMode)
				sceneRenderer->sceneTool->mode = ROTATE;

			if (ImGui::IsKeyDown(GLFW_KEY_T) && !travelMode)
				sceneRenderer->sceneTool->mode = MOVE;

			if (!toolMode && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(ImGuiMouse_Left_))
			{
				if (sceneRenderer->RenderForObjectTools(x, y))
				{
					toolMode = true;
				}
			}

			if (toolMode)
			{
				glm::vec3 normal(0, 0, 1);
				if ((sceneRenderer->sceneTool->modedirection & TOOLZ))
					normal = { 0,1,0 };
				glm::vec2 size; size.x = viewportSize.x; size.y = viewportSize.y;

				sceneRenderer->sceneTool->transObjects(sceneRenderer->selectedActor->transformation, x, y, io.MouseDelta.x, io.MouseDelta.y,
					sceneRenderer->sceneCamera->position, sceneRenderer->sceneCamera->screenToWorld(x, y, normal, size));

				sceneRenderer->selectedActor->processTransformation();
			}

			if (toolMode && ImGui::IsMouseReleased(ImGuiMouse_Left_))
			{
				toolMode = false;
			}

			if (!toolMode && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(ImGuiMouse_Left_))
			{
				sceneRenderer->selectedActor = sceneRenderer->RenderForObjectPicker(x, y);
			}
		}

		sceneRenderer->render();

		ImGui::Image((void*)sceneRenderer->GetTextureColorBuffer(), viewportSize, { 0,0 }, { viewportSize.x / sceneMaxWidth, viewportSize.y / sceneMaxHeight });

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
				midx = (int)(ImGui::GetWindowPos().x + viewportSize.x / 2);
				midy = (int)(ImGui::GetWindowPos().y + viewportSize.y / 2);

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
				midx = (int)(ImGui::GetWindowPos().x + viewportSize.x / 2);
				midy = (int)(ImGui::GetWindowPos().y + viewportSize.y / 2);

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
			ImGui::DragInt("Line Count", &(sceneRenderer->grid->lineCount), 1, 2, 1000);
			if (ImGui::DragFloat("Space Lenght", &(sceneRenderer->grid->spaceLength), 1.0f, 0.01f, 1000.f) && !sceneRenderer->grid->spaceLength)
				sceneRenderer->grid->spaceLength = 1;
			ImGui::ColorEdit3("Grid Color", (sceneRenderer->grid->gridColor));
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
		if (!sceneRenderer->selectedActor)
			ImGui::Text("Please Select a Actor");
		else
		{
			ImGui::Text(sceneRenderer->selectedActor->name.c_str());
			if (ImGui::CollapsingHeader("Transformations", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::DragFloat3("Position", &sceneRenderer->selectedActor->transformation->position[0], 0.1f, -30000.f, 30000.f))
					sceneRenderer->selectedActor->processTransformation();
				if (ImGui::DragFloat3("Rotation Eular", &sceneRenderer->selectedActor->transformation->eRotation[0], 1.f, -359.99999f, 359.99999f))
				{
					sceneRenderer->selectedActor->transformation->calcQuatFromEuler();
					sceneRenderer->selectedActor->processTransformation();
				}

				if (ImGui::DragFloat4("Rotation Quat", &sceneRenderer->selectedActor->transformation->qRotation[0], 1.f, -30000.f, 30000.f))
				{
					sceneRenderer->selectedActor->transformation->calcEulerFromQuat();
					sceneRenderer->selectedActor->processTransformation();
				}
				if (ImGui::DragFloat3("Scale", &sceneRenderer->selectedActor->transformation->scale[0], 1.f, -30000.f, 30000.f))
				{
					sceneRenderer->selectedActor->processTransformation();
				}

			}
		}
	}
	ImGui::End();


	ImGui::Begin("Project Explorer", NULL);
	ImGui::End();

	ImGui::Begin("Hierarchy", NULL);
	{
		DrawHierarchy(scene->rootActor);
	}
	ImGui::End();

	ImGui::Begin("Create Object", NULL);
	ImGui::End();

}

void Editor::DrawHierarchy(Actor *root)

{
	int n;
	ImGuiTreeNodeFlags src_flags = 0;
	if (root->numberOfChildren == 0)
		src_flags = ImGuiTreeNodeFlags_Leaf;
	else
		src_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow;
	if (sceneRenderer->selectedActor == root)
		src_flags |= ImGuiTreeNodeFlags_Selected;

	bool node_open = ImGui::TreeNodeEx((void *)root, src_flags, root->name.c_str());

	//right click popup
	{
		ImGui::PushID(root->name.c_str());
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete") && root->name != "root")
			{
				root->RemoveActor();
				sceneRenderer->selectedActor = NULL;
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
					sceneRenderer->hoveredActor = root;
				}

				ImGui::SetDragDropPayload("DND_DEMO_NAME", &n, sizeof(int));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME"))
				{
					if (sceneRenderer->hoveredActor)
						sceneRenderer->hoveredActor->AddParent(root);
					sceneRenderer->hoveredActor = NULL;
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::IsItemClicked() && sceneRenderer->gamebase->currentScene->rootActor != root)
		{
			sceneRenderer->selectedActor = root;
		}

		if (node_open)
		{
			for (int i = 0; i < root->numberOfChildren; i++)
				DrawHierarchy(root->children[i]);
			ImGui::TreePop();
		}
	}
}

void Editor::handle_dropped_file(const char * path)
{


	std::string str = path;
	std::string extension = str.substr(str.find_last_of('.') + 1);
	if (extension == "fbx")
	{
		Prefab *p = ModelLoader::loadPrefab(path);
		int a = 3;
		a = 5;

		scene->addActor(p, glm::vec3(1));

		a = 5;
	}
}


