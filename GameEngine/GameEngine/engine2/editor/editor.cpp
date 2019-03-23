#include "editor.h"
#include<Api.h>
#include<engine/ActorManager.h>
#include<stack>
#define stringify( name ) # name

Editor::Editor(GLFWwindow *window)
{
	this->window = window;

	projectManager = new ProjectManager("Kero Game 1", "C:\\GameEngine\\");
	Scene *scene = new Scene("Taso", projectManager);
	projectManager->add(scene);
	gameBase = new GameBase(scene);
	sceneRenderer = new SceneRenderer(gameBase);
	auto as = new AnimationSystem(gameBase);
	sm = new SystemManager(as);
	Serializable::Save(projectManager, projectManager->path.c_str());

	
	

	loadIcons();


}
void Editor::loadIcons()
{
	Texture *t = new Texture("ConstantIcons\\scene.png");
	t->loadTexture();
	sceneIcon = (void *)t->textureID;
	delete t;
}
void Editor::ShowComponentList()
{
	
			
	auto cob = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->componentObject;

	if (LightComponent *lightcomp = cob->getComponent<LightComponent>())
	{
		if (ImGui::CollapsingHeader("Light Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginCombo("##lighttype", LightTypeNames[lightcomp->lightType])) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < IM_ARRAYSIZE(LightTypeNames); n++)
				{
					if (ImGui::Selectable(LightTypeNames[n]))
						lightcomp->lightType = (LightType)n;
				}
				ImGui::EndCombo();
			}

			ImGui::ColorEdit4("Light Color##LightComponent", (float*)&lightcomp->color, ImGuiColorEditFlags_NoInputs);
			ImGui::DragFloat("Intensity##LightComponent", &lightcomp->intensity, 0.05f, 0.05f,100.f);

			if (lightcomp->lightType == LightType::Spotlight)
				ImGui::DragFloat("Angle##LightComponent", &lightcomp->angle, 0.05f, 0.05f, 100.f);

			if (!(lightcomp->lightType == LightType::Directional))
			{
				if (ImGui::DragFloat("Distance##LightComponent", &lightcomp->distance, 0.05f, 0,10000.f))
					lightcomp->calculateAttenuation();
			}
		}
	}

	if (ModelComponent *modelcomp = cob->getComponent<ModelComponent>())
	{
		if (ImGui::CollapsingHeader("Model Component", ImGuiTreeNodeFlags_DefaultOpen))
		{

			if (ImGui::BeginCombo(("Select Model##selectmodel"), modelcomp->model->name.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto model : projectManager->models)
				{
					if (ImGui::Selectable(model->name.c_str()))
					{
						gameBase->currentScene->componentSystem->changeModel<ModelComponent>(sceneRenderer->selectedActor, model);
						modelcomp->model = model;
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::InputInt("Number of Materials##numberofmaterials", &modelcomp->numberOfMaterials))
				modelcomp->materials.resize(modelcomp->numberOfMaterials);

			for (int i = 0; i < modelcomp->numberOfMaterials; i++)
			{ //meshleri dön

				const char *name = "No Material";
				if (modelcomp->materials[i])
					name = modelcomp->materials[i]->name.c_str();

				if (ImGui::BeginCombo(("Material" + std::to_string(i) + "##choicematerial").c_str(), name)) // The second parameter is the label previewed before opening the combo.
				{
					for (auto material : projectManager->materials)
					{//projedeki bütün materialyerlleri göster
						if (ImGui::Selectable(material->name.c_str()))
							modelcomp->materials.insert(modelcomp->materials.begin(), 1, material);
					}
					ImGui::EndCombo();
				}

			}
		}

	}

	if (SkinnedModelComponent *modelcomp = cob->getComponent<SkinnedModelComponent>())
	{
		if (ImGui::CollapsingHeader("Skinned Model Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			Actor *root = GE_Engine->actorManager->GetActor(modelcomp->rootBone);
			if (ImGui::BeginCombo(("Select RootBone##selectbone"), root ? root->name.c_str() : "Select RootBone")) // The second parameter is the label previewed before opening the combo.
			{
				std::stack<Actor*> stack;


				stack.push(GE_Engine->actorManager->GetActor(gameBase->currentScene->rootActor));
				Actor *s;
				while (!stack.empty())
				{
					
					s = stack.top();
					stack.pop();
		
					if (ImGui::Selectable((s->name + "##selectbonee").c_str()))
					{
						modelcomp->rootBone = s->actorID;
						modelcomp->matchBones();
					}

					for(int i = 0 ; i< s->numberOfChildren; i++)
							stack.push(GE_Engine->actorManager->GetActor(s->children[i]));
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo(("Select Model##selectmodel"), modelcomp->model->name.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto model : projectManager->models)
				{
					if (ImGui::Selectable(model->name.c_str()))
					{
						gameBase->currentScene->componentSystem->changeModel<SkinnedModelComponent>(sceneRenderer->selectedActor, model);
						modelcomp->model = model;
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::InputInt("Number of Materials##numberofmaterials", &modelcomp->numberOfMaterials))
				modelcomp->materials.resize(modelcomp->numberOfMaterials);

			for (int i = 0; i < modelcomp->numberOfMaterials; i++)
			{ //meshleri dön

				const char *name = "No Material";
				if (modelcomp->materials[i])
					name = modelcomp->materials[i]->name.c_str();

				if (ImGui::BeginCombo(("Material" + std::to_string(i) + "##choicematerial").c_str(), name)) // The second parameter is the label previewed before opening the combo.
				{
					for (auto material : projectManager->materials)
					{//projedeki bütün materialyerlleri göster
						if (ImGui::Selectable(material->name.c_str()))
							modelcomp->materials.insert(modelcomp->materials.begin(), 1, material);
					}
					ImGui::EndCombo();
				}

			}
		}

	}

	if (AnimatorComponent *animator = cob->getComponent<AnimatorComponent>())
	{
		if (ImGui::CollapsingHeader("Animator Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginCombo(("Select Animation##selectanimation"), !animator->currentAnimation ? "" : animator->currentAnimation->name.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto anim : projectManager->animations)
				{
					if (ImGui::Selectable(anim->name.c_str()))
					{
						animator->currentAnimation = anim;
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("play animation loop"))
			{
				if(animator->currentAnimation)
					animator->PlayLoop(animator->currentAnimation);
			}
			ImGui::SameLine();
			if (ImGui::Button("play animation once"))
			{
				if (animator->currentAnimation)
					animator->PlayOnce(animator->currentAnimation);
			}
			if (ImGui::Button("stop animation"))
			{
				if (animator->currentAnimation)
					animator->state = false;
			}
		}

	}


		
}

void Editor::ObjectProperties()
{
	ImGui::Begin("Object Properties", NULL);
	{
		Actor * selectedActor = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor);
		if (!selectedActor)
			ImGui::Text("Please Select a Actor");
		else
		{
			ImGui::Text(selectedActor->name.c_str());
			if (ImGui::CollapsingHeader("Transformations", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::DragFloat3("Position", &selectedActor->transformation->localPosition[0], 0.1f, -30000.f, 30000.f))
					selectedActor->processTransformation();
				if (ImGui::DragFloat3("Rotation Eular", &selectedActor->transformation->localeulerRotation[0], 1.f, -359.99999f, 359.99999f))
				{
					selectedActor->transformation->calcQuatFromEuler();
					selectedActor->processTransformation();
				}

				if (ImGui::DragFloat4("Rotation Quat", &selectedActor->transformation->localquatRotation[0], 1.f, -30000.f, 30000.f))
				{
					selectedActor->transformation->calcEulerFromQuat();
					selectedActor->processTransformation();
				}
				if (ImGui::DragFloat3("Scale", &selectedActor->transformation->localScale[0], 1.f, -30000.f, 30000.f))
				{
					selectedActor->processTransformation();
				}

				ShowComponentList();

				ImGui::Separator();
				
				ObjectPropertiesMaterials();
				
				const char *items[] = {"Light Component", "Animator Component"};
				if (ImGui::BeginCombo("##addcomponent", "Add Component", ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						if (ImGui::Selectable(items[n]))
							switch (n)
							{
							case 0://light component
							{
								IComponent *ic = (IComponent*)new LightComponent(sceneRenderer->selectedActor);
								selectedActor->AddComponent<LightComponent>(ic);
							}break;

							case 1:
							{
								IComponent *ic = (IComponent*)new AnimatorComponent(sceneRenderer->selectedActor);
								selectedActor->AddComponent<AnimatorComponent>(ic);

							}break;

							default:
								break;
							}
					}
					ImGui::EndCombo();
				}
			}
		}
	}
	ImGui::End();
}

void Editor::ObjectPropertiesMaterials()
{
	
	if (sceneRenderer->selectedActor)
	{
		bool flag = false;

		ModelComponent *mcmp = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->componentObject->getComponent<ModelComponent>();
		if (mcmp)
		{
			for (int i = 0; i < mcmp->numberOfMaterials; i++)
			{
				if (Material *m = mcmp->materials[i])
				{
					flag = true;
					if (ImGui::CollapsingHeader(("Material " + m->name +"##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::ColorEdit3(("Ambient Color##Material" + std::to_string(i)).c_str(), (float*)&m->ambientColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_RGB);
					}
				}
			}
		}
		if (flag)
			ImGui::Separator();
	}
	
}


Editor::~Editor()
{
}

void Editor::Render()
{
	sm->work();
	Actor *root = GE_Engine->actorManager->GetActor(gameBase->currentScene->rootActor);
	for (int i = 0; i < root->numberOfChildren; i++)
		GE_Engine->actorManager->GetActor(root->children[i])->processTransformation();


	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking ;
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
			sceneRenderer->selectedActor = ActorID::INVALID_HANDLE;
		}

		if (ImGui::IsKeyPressed(GLFW_KEY_F) && sceneRenderer->selectedActor)
		{
			sceneRenderer->focusActor(sceneRenderer->selectedActor);
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
				Actor * selectedActor  = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor);
				glm::vec3 normal(0, 0, 1);
				if ((sceneRenderer->sceneTool->modedirection & TOOLZ))
					normal = { 0,1,0 };
				glm::vec2 size; size.x = viewportSize.x; size.y = viewportSize.y;

				sceneRenderer->sceneTool->transObjects(selectedActor->transformation, x, y, io.MouseDelta.x, io.MouseDelta.y,
					sceneRenderer->sceneCamera->position, sceneRenderer->sceneCamera->screenToWorld(x, y, normal, size));

				//selectedActor->processTransformation();
				
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
		std::vector<bool> isopen;
		float test=0;
		{
			
			if (!projectManager->animations.empty())
				isopen.resize(projectManager->animations[0]->animationNodeMap.size()*3,false);
			
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
			ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.2f, ImGui::GetWindowContentRegionMax().y-30), false, window_flags);
			{
				if (!projectManager->animations.empty())
				{
					int counter = 0;
					ImGui::SetCursorPosY(15);
					for (auto node : projectManager->animations[0]->animationNodeMap)
					{
						if (ImGui::TreeNodeEx((void *)&node.second->positionKeys, ImGuiTreeNodeFlags_OpenOnArrow, (node.first + ": Position").c_str()))
						{
							isopen[counter * 3] = true;
							ImGui::Text((std::to_string(node.second->positionKeys[0].second.x).c_str()));
							ImGui::Text((std::to_string(node.second->positionKeys[0].second.y).c_str()));
							ImGui::Text((std::to_string(node.second->positionKeys[0].second.z).c_str()));
							ImGui::TreePop();
						}
						if (ImGui::TreeNodeEx((void *)&node.second->rotationKeys, 0, (node.first + ": Rotation").c_str()))
						{
							isopen[counter * 3 + 1] = true;
							ImGui::Text((std::to_string(node.second->rotationKeys[0].second.x).c_str()));
							ImGui::Text((std::to_string(node.second->rotationKeys[0].second.y).c_str()));
							ImGui::Text((std::to_string(node.second->rotationKeys[0].second.z).c_str()));
							ImGui::Text((std::to_string(node.second->rotationKeys[0].second.w).c_str()));
							ImGui::TreePop();
						}
						if (ImGui::TreeNodeEx((void *)&node.second->scaleKeys, 0, (node.first + ": Scale").c_str()))
						{
							isopen[counter * 3 + 2] = true;
							ImGui::Text((std::to_string(node.second->scaleKeys[0].second.x).c_str()));
							ImGui::Text((std::to_string(node.second->scaleKeys[0].second.y).c_str()));
							ImGui::Text((std::to_string(node.second->scaleKeys[0].second.z).c_str()));
							ImGui::TreePop();
						}

						counter++;

					}
				}
				test = ImGui::GetScrollY();
				
			}
			ImGui::EndChild();
		}

		ImGui::SameLine();

		// Child 2: rounded border
		{
			static float width = 5000;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
			ImGui::BeginChild("Channels", ImVec2(0, ImGui::GetWindowContentRegionMax().y-30), false, window_flags);
			{			
				ImGui::SetNextWindowContentSizeX(width);
				if (ImGui::BeginTimeline("asdasd##asdasd", 10000))
				{
					ImGui::LINE_COUNT = width / 50;
					if (ImGui::IsMouseHoveringWindow() && io.MouseWheel != 0)
						width += io.MouseWheel * 200;
					if (width < ImGui::GetWindowSize().x / 2)
						width = ImGui::GetWindowSize().x / 2;

					ImGui::SetScrollY(test);

					if (!projectManager->animations.empty()) {
						unsigned int i = 0;
						int counter = 0;
					
						i = 1;

						for (auto node : projectManager->animations[0]->animationNodeMap)
						{
							for (auto pos : node.second->positionKeys)
							{
								ImGui::TimelineEvent(node.first.c_str(), &pos.first, i);
							}
							if (isopen[counter*3])
								i += 3;
							i++;

							for (auto pos : node.second->rotationKeys)
							{
								ImGui::TimelineEvent(node.first.c_str(), &pos.first, i);
							}
							if (isopen[counter * 3 + 1])
								i += 4;
							i++;

							for (auto pos : node.second->scaleKeys)
							{
								ImGui::TimelineEvent(node.first.c_str(), &pos.first, i);
							}
							if (isopen[counter*3+2])
								i += 3;
							i++;
						

							counter++;
						}

					}
				}
				ImGui::EndTimeline();
			}
			ImGui::EndChild();
		//	ImGui::PopStyleVar();
		}


			
		
		
		
	}
	ImGui::End();

	this->ObjectProperties();

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


	


	DrawProjectExplorer();

	ImGui::Begin("Hierarchy", NULL);
	{
		DrawHierarchy(gameBase->currentScene->rootActor);
	}
	ImGui::End();

	ImGui::Begin("Create Object", NULL);
	ImGui::End();

}

void Editor::DrawHierarchy(ActorID rootid)

{
	int n;
	ImGuiTreeNodeFlags src_flags = 0;
	Actor *root = GE_Engine->actorManager->GetActor(rootid);
	if (root->numberOfChildren == 0)
		src_flags = ImGuiTreeNodeFlags_Leaf;
	else
		src_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow;
	if (sceneRenderer->selectedActor == rootid)
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
				sceneRenderer->selectedActor = ActorID::INVALID_HANDLE;
				ImGui::EndPopup();
				ImGui::PopID();
				if(node_open)
					ImGui::TreePop();
				return;
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
					sceneRenderer->hoveredActor = rootid;
				}

				ImGui::SetDragDropPayload("DND_DEMO_NAME", &n, sizeof(int));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME"))
				{
					Actor *hoveredActor = GE_Engine->actorManager->GetActor(sceneRenderer->hoveredActor);
					if (hoveredActor)
						hoveredActor->AddParent(rootid);
					sceneRenderer->hoveredActor = ActorID::INVALID_HANDLE;
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::IsItemClicked() && sceneRenderer->gamebase->currentScene->rootActor != rootid)
		{
			sceneRenderer->selectedActor = rootid;
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
		Prefab *p = ModelLoader::loadPrefab(path, projectManager);
		int a = 3;
		a = 5;

		gameBase->currentScene->addActor(p, glm::vec3(1));

		a = 5;
	}
}

void Editor::DrawProjectExplorer()
{
	ImGui::Begin("Project Explorer", NULL);
	{
		int count = 0;
		int i = 0;
		count += projectManager->scenes.size();
		count += projectManager->models.size();
		count += projectManager->textures.size();
		count += projectManager->prefabs.size();
		count += projectManager->materials.size();
		count += projectManager->animations.size();
		for (auto scene : projectManager->scenes)
		{
			if (DrawSingleProjectItem(sceneIcon, scene->name, i++, count))
			{
				ImGui::SetTooltip("Scene");
			}
		}
		for (auto model : projectManager->models)
		{
			if(DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), model->name, i++, count))
			{
				ImGui::SetTooltip("Model");
			}
		}
		for (auto texture : projectManager->textures)
		{
			if (DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), texture->name, i++, count))
			{
				ImGui::SetTooltip("Texture");
			}
		}
		for (auto prefab : projectManager->prefabs)
		{
			if (DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), prefab->name, i++, count))
			{
				ImGui::SetTooltip("Prefab");
			}
		}
		for (auto material : projectManager->materials)
		{
			if (DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), material->name, i++, count))
			{
				ImGui::SetTooltip("Material");
			}
		}
		for (auto animation : projectManager->animations)
		{
			if (DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), animation->name, i++, count))
			{
				ImGui::SetTooltip("Animation");
			}
		}

		


		
	}
	ImGui::End();
}

bool Editor::DrawSingleProjectItem(void * image, std::string name, int n, int buttons_count)
{
	bool isHovered = false;
	ImGuiStyle& style = ImGui::GetStyle();
	
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	
	ImGui::PushID(n);
	{
		ImGui::BeginGroup();
		{

			ImVec2 cursorBegin = ImGui::GetCursorPos();
			ImGui::Button("", button_sz);

			ImVec2 cursorEnd = ImGui::GetCursorPos();
			ImGui::SetCursorPos(cursorBegin);
			ImGui::ImageButton(image, ImVec2(button_sz.x - 4, button_sz.y * 6 / 8), { 0,0 }, { 1,1 }, 2);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
			ImGui::Text(std::string(name).substr(0, 10).c_str());
			
		}
		ImGui::EndGroup();
		isHovered = ImGui::IsItemHovered();
	}
	float last_button_x2 = ImGui::GetItemRectMax().x;
	float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
	if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
		ImGui::SameLine();
	ImGui::PopID();

	return isHovered;
}


