#include "editor.h"
#include<Api.h>
#include<engine/ActorManager.h>
#include<stack>
#define stringify( name ) # name

#define StateSize 1000000

Editor::Editor(GLFWwindow *window) : isPlaying(false)
{
	this->window = window;

	projectManager = new ProjectManager("Kero Game 1", "C:\\GameEngine\\");
	GE_Engine->mainPath = projectManager->path.data();
	ScriptHelper::moveMainAssembly();


	Scene *scene = new Scene("Taso", projectManager);
	projectManager->add(scene);
	gameBase = new GameBase(scene);
	sceneRenderer = new SceneRenderer(gameBase);

	Serializable::Save(projectManager, projectManager->path.c_str());



}

void Editor::ShowComponentList()
{
	
			
	auto actor = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor);

	if (LightComponent *lightcomp = actor->GetComponent<LightComponent>())
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

	if (ModelComponent *modelcomp = actor->GetComponent<ModelComponent>())
	{
		if (ImGui::CollapsingHeader("Model Component", ImGuiTreeNodeFlags_DefaultOpen))
		{

			if (ImGui::BeginCombo(("Select Model##selectmodel"), modelcomp->getModel()->name.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto model : projectManager->models)
				{
					if (ImGui::Selectable(model->name.c_str()))
					{
						GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->GetComponent<ModelComponent>()->setModel(model);
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

	if (SkinnedModelComponent *modelcomp = actor->GetComponent<SkinnedModelComponent>())
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
			if (ImGui::BeginCombo(("Select Model##selectmodel"), modelcomp->getModel()->name.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (auto model : projectManager->models)
				{
					if (ImGui::Selectable(model->name.c_str()))
					{
						GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->GetComponent<SkinnedModelComponent>()->setModel(model);
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

	if (AnimatorComponent *animator = actor->GetComponent<AnimatorComponent>())
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

	if (SphereColliderComponent *spherecollider = actor->GetComponent<SphereColliderComponent>())
	{
		if (ImGui::CollapsingHeader("SphereCollider Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat("Radius##SphereColliderRadius", &spherecollider->geometry.radius, 0.1, 0.1, 100.f))
				spherecollider->update();
		}

	}

	if (CapsuleColliderComponent *capsulecollider = actor->GetComponent<CapsuleColliderComponent>())
	{
		if (ImGui::CollapsingHeader("CapsuleCollider Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderInt("Up",&capsulecollider->upp,0,2))
				capsulecollider->update();
			if (ImGui::DragFloat("Radius##CapsuleColliderRadiues", &capsulecollider->geometry.radius, 0.1, 0.1, 100.f))
				capsulecollider->update();
			if (ImGui::DragFloat("HalfHeight##CapsuleColliderRadiues", &capsulecollider->geometry.halfHeight, 0.1, 0.1, 100.f))
				capsulecollider->update();
		}

	}
		
	if (CubeColliderComponent *cubecollider = actor->GetComponent<CubeColliderComponent>())
	{
		if (ImGui::CollapsingHeader("CubeCollider Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat("X##CubeColliderX", &cubecollider->geometry.halfExtents.x, 0.1, 0.1, 100.f))
				cubecollider->update();
			if(ImGui::DragFloat("Y##CubeColliderY", &cubecollider->geometry.halfExtents.y, 0.1, 0.1, 100.f))
				cubecollider->update();
			if(ImGui::DragFloat("Z##CubeColliderZ", &cubecollider->geometry.halfExtents.z, 0.1, 0.1, 100.f))
				cubecollider->update();
		}

	}

	if (RigidBodyComponent *rigidbody = actor->GetComponent<RigidBodyComponent>())
	{
		if (ImGui::CollapsingHeader("RigidBody Component", ImGuiTreeNodeFlags_DefaultOpen))
		{
			
		}

	}
	if (ScriptComponent *scriptcomp = actor->GetComponent<ScriptComponent>())
	{
		if (ImGui::CollapsingHeader("Script Component", ImGuiTreeNodeFlags_DefaultOpen))
		{

			for (auto script : scriptcomp->scripts)
			{
				ImGui::Text((script->name).c_str());
				ImGui::SameLine(ImGui::GetWindowWidth() -30);
				if (ImGui::Button(("X##"+script->name).c_str()))
				{
					scriptcomp->remove(script);
				}
			}

			ImGui::Separator();
			static char scriptname[50];			
			ImGui::InputText("##scriptname",scriptname,50);
			ImGui::SameLine();
			Script *s = new Script(scriptname);
			if (ImGui::Button("Add New Script") && strlen(scriptname) > 5 && !projectManager->isExists(s))
			{

				projectManager->add(s);
				scriptcomp->add(s);
				ScriptHelper::createScript(s);
				scriptname[0] = 0;
			}
			else
				free(s);
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
				if (ImGui::DragFloat3("Position", &selectedActor->transformation.localPosition[0], 0.1f, -30000.f, 30000.f))
				{
					selectedActor->processTransformation();
					selectedActor->transformation.applyPhysic();
				}
				if (ImGui::DragFloat3("Rotation Eular", &selectedActor->transformation.localeulerRotation[0], 1.f, -359.99999f, 359.99999f))
				{
					selectedActor->transformation.calcQuatFromEuler();
					selectedActor->processTransformation();
					selectedActor->transformation.applyPhysic();
				}

				if (ImGui::DragFloat4("Rotation Quat", &selectedActor->transformation.localquatRotation[0], 1.f, -30000.f, 30000.f))
				{
					selectedActor->transformation.calcEulerFromQuat();
					selectedActor->processTransformation();
					selectedActor->transformation.applyPhysic();
				}
				if (ImGui::DragFloat3("Scale", &selectedActor->transformation.localScale[0], 1.f, -30000.f, 30000.f))
				{
					selectedActor->processTransformation();
					selectedActor->transformation.applyPhysic();
				}

				ShowComponentList();

				ImGui::Separator();
				
				ObjectPropertiesMaterials();
				
				const char *items[] = {"Light Component", "Animator Component", "SphereCollider Component", "CapsuleCollider Component", "CubeCollider Component","RigidBody Component", "Script Component" };
				if (ImGui::BeginCombo("##addcomponent", "Add Component", ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						if (ImGui::Selectable(items[n]))
							switch (n)
							{
							case 0://light component
							{
								selectedActor->AddComponent<LightComponent>();
							}break;

							case 1:
							{
								selectedActor->AddComponent<AnimatorComponent>();
							}break;
							case 2:
							{
								selectedActor->AddComponent<SphereColliderComponent>();
							}break;
							case 3:
							{
								selectedActor->AddComponent<CapsuleColliderComponent>();
							}break;
							case 4:
							{
								selectedActor->AddComponent<CubeColliderComponent>();
							}break;
							case 5:
							{
								selectedActor->AddComponent<RigidBodyComponent>();
							}break;
							case 6:
							{
								selectedActor->AddComponent<ScriptComponent>();
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
	
	if (sceneRenderer->selectedActor != ActorID::INVALID_HANDLE)
	{
		bool flag = false;

		IModelComponent *mcmp = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->GetComponent<ModelComponent>();
		if(!mcmp)
			mcmp = GE_Engine->actorManager->GetActor(sceneRenderer->selectedActor)->GetComponent<SkinnedModelComponent>();
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
					
						const char *name = "No Texture";
						if (m->ambientTexture)
							name = m->ambientTexture->name.c_str();

						if (ImGui::BeginCombo(("Texture" + std::to_string(i) + "##choiceTexture").c_str(), name)) // The second parameter is the label previewed before opening the combo.
						{
							for (auto texture : projectManager->textures)
							{//projedeki bütün materialyerlleri göster
								if (ImGui::Selectable(texture->name.c_str()))
									m->ambientTexture = texture;
							}
							ImGui::EndCombo();
						}

						ImGui::Checkbox("Trasparent##MaterialTransparent", &m->isTransparent);
					
					
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
	GE_Engine->sytemManager->work();
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

	ImGui::Begin("ToolBar", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	{
		if (ImGui::ButtonEx("Play", { 0,0 }, isPlaying ? ImGuiButtonFlags_Disabled : 0))
		{
			isPlaying = true;
			GE_Engine->physicSystem->enabled = true;
			GE_Engine->scriptSystem->initSystem();
			GE_Engine->scriptSystem->startSytem();
			
		}
		ImGui::SameLine();
		if (ImGui::ButtonEx("Stop", { 0,0 }, !isPlaying ? ImGuiButtonFlags_Disabled : 0))
		{
			isPlaying = false;
			GE_Engine->physicSystem->enabled = false;
			GE_Engine->scriptSystem->freeSystem();
		
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 90);
		if (ImGui::ButtonEx("Compile", { 0,0 }, isPlaying ? ImGuiButtonFlags_Disabled : 0))
		{
			ScriptHelper::compile();

		}
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

				sceneRenderer->sceneTool->transObjects(&selectedActor->transformation, x, y, io.MouseDelta.x, io.MouseDelta.y,
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
	static ActorID renameactor = ActorID::INVALID_HANDLE;
	static char name[50];
	int n;
	ImGuiTreeNodeFlags src_flags = 0;
	Actor *root = GE_Engine->actorManager->GetActor(rootid);
	if (root->numberOfChildren == 0)
		src_flags = ImGuiTreeNodeFlags_Leaf;
	else
		src_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow;
	if (sceneRenderer->selectedActor == rootid)
		src_flags |= ImGuiTreeNodeFlags_Selected;
	
	static ImVec2 x;
	if (renameactor == rootid)
	{
		x = ImGui::GetCursorPos();
		ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,0 });
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { 1,1,1,0 });
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, { 1,1,1,0 });
	}
	
	bool node_open = ImGui::TreeNodeEx((void *)root, src_flags, root->name.c_str());

	if (renameactor == rootid)
	{
		
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::SetCursorPos(x);
		ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
		if (ImGui::IsKeyPressed(GLFW_KEY_ENTER))
		{
			root->name = name;
			renameactor = ActorID::INVALID_HANDLE;
		}
	}


	//right click popup
	{
		ImGui::PushID(root->name.c_str());
		if (ImGui::BeginPopupContextItem() )
		{
	
			bool isroot = (rootid != sceneRenderer->gamebase->currentScene->rootActor);
			if ((rootid != sceneRenderer->gamebase->currentScene->rootActor))
			{
				if (ImGui::MenuItem("Delete"))
				{
					root->RemoveActor();
				sceneRenderer->selectedActor = ActorID::INVALID_HANDLE;
				ImGui::EndPopup();
				ImGui::PopID();
				if (node_open)
					ImGui::TreePop();
				return;
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, {0.3,0.3,0.3,1.0});
				ImGui::Selectable("Delete");
				ImGui::PopStyleColor();
			}
			if (rootid != sceneRenderer->gamebase->currentScene->rootActor)
			{
				if (ImGui::MenuItem("Rename"))
				{
					renameactor = rootid;
					strcpy_s(name, root->name.c_str());
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.3,0.3,0.3,1.0 });
				ImGui::Selectable("Rename");
				ImGui::PopStyleColor();
			}
			
			ImGui::Separator();
			if (ImGui::MenuItem("Empty")) {
				ActorID newactor = GE_Engine->actorManager->CreateActor("Empty Actor", sceneRenderer->gamebase->currentScene);
				GE_Engine->actorManager->GetActor(newactor)->AddParent(rootid);
			}
			if (ImGui::BeginMenu("3D Models"))
			{
				if (ImGui::MenuItem("Cube")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Cube", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);

					actor->AddComponent<ModelComponent>(ConstantModels::getCubeModel());
				}
				if (ImGui::MenuItem("Sphere")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Sphere", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);

					actor->AddComponent<ModelComponent>(ConstantModels::getSphereModel());
				}
				if (ImGui::MenuItem("Capsule")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Capsule", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);
					actor->AddComponent<ModelComponent>(ConstantModels::getCapsuleModel());
				}
				if (ImGui::MenuItem("Plane")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Plane", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);
					actor->AddComponent<ModelComponent>(ConstantModels::getPlaneModel());
				}
				if (ImGui::MenuItem("Cone")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Cone", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);
					actor->AddComponent<ModelComponent>(ConstantModels::getConeModel());
				}
				if (ImGui::MenuItem("Cylinder")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("Cylinder", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);
					actor->AddComponent<ModelComponent>(ConstantModels::getCylinderModel());
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Lights"))
			{
				if (ImGui::MenuItem("Point")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("PointLight", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);

					actor->AddComponent<LightComponent>(Point);
					
				}
				if (ImGui::MenuItem("Directional")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("DirectionalLight", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);

					actor->AddComponent<LightComponent>(Directional);
				}
				if (ImGui::MenuItem("Spot")) {
					ActorID newactor = GE_Engine->actorManager->CreateActor("SpotLight", sceneRenderer->gamebase->currentScene);
					Actor *actor = GE_Engine->actorManager->GetActor(newactor);
					actor->AddParent(rootid);

					actor->AddComponent<LightComponent>(Spotlight);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Camera")) {
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
		static Texture *selectedTexture;
		static bool opentexture = true;
		int count = 0;
		int i = 0;
		count += projectManager->scenes.size();
		count += projectManager->models.size();
		count += projectManager->textures.size();
		count += projectManager->prefabs.size();
		count += projectManager->materials.size();
		count += projectManager->animations.size();
		count += projectManager->scripts.size();
		for (auto scene : projectManager->scenes)
		{
			if (DrawSingleProjectItem((void *)ConstantTextures::Textures::sceneTexture->gettextureID(), scene->name, i++, count))
			{
				ImGui::SetTooltip(("Scene -> " + scene->name).c_str());
			}
		}
		for (auto model : projectManager->models)
		{
			if(DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), model->name, i++, count))
			{
				ImGui::SetTooltip(("Model -> " + model->name).c_str());
			}
		}
		for (auto texture : projectManager->textures)
		{
			static bool clicked;
			if (DrawSingleProjectItem((void *)texture->gettextureID(), texture->name, i++, count, &clicked))
			{
				ImGui::SetTooltip("Texture");
			}
			if (clicked)
			{
				selectedTexture = texture;
				opentexture = true;
			}
		}
		for (auto prefab : projectManager->prefabs)
		{
			if (DrawSingleProjectItem((void *)sceneRenderer->GetTextureColorBuffer(), prefab->name, i++, count))
			{
				ImGui::SetTooltip("Prefab");
				opentexture = true;
			}
		}
		for (auto material : projectManager->materials)
		{
			if (material->ambientTexture)
			{
				if (DrawSingleProjectItem((void *)material->ambientTexture->gettextureID(), material->name, i++, count))
					ImGui::SetTooltip(("Material -> " + material->name).c_str());
			}
			else if (DrawSingleProjectItem(material->ambientColor, material->name, i++, count))
			{
				ImGui::SetTooltip(("Material -> " + material->name).c_str());
			}
		}
		for (auto animation : projectManager->animations)
		{
			if (DrawSingleProjectItem((void *)ConstantTextures::Textures::animationTexture->gettextureID(), animation->name, i++, count))
			{
				ImGui::SetTooltip(("Animation -> " + animation->name).c_str() );
			}
		}
		for (auto script : projectManager->scripts)
		{
			if (DrawSingleProjectItem((void *)ConstantTextures::Textures::scriptTexture->gettextureID(), script->name, i++, count))
			{
				ImGui::SetTooltip(("Script -> " + script->name).c_str());
			}
		}

		
		if (selectedTexture)
		{
			{
				
				if (ImGui::Begin((selectedTexture->name + " | Texture Settings").c_str(), &opentexture))
				{
					if (selectedTexture->gettextureID() == Texture::INVALID_TEXTURE_ID)
						ImGui::Text("Texture Not Found");
					else
					{
						ImGui::ImageButton((void *)selectedTexture->gettextureID(), {300,300}, { 0,0 }, { 1,1 }, 2);
						ImGui::Text(selectedTexture->name.c_str());
						if (ImGui::Button("Change Texture"))
						{
							auto selection = pfd::open_file("Select a texture", ".",
								{ "Image Files", "*.png *.jpg *.jpeg *.bmp",}).result();
							if (!selection.empty())
							{
								selectedTexture->unload();
								GE_Engine->resourceManager->changeKey<Texture>(selectedTexture->fullpath, selection[0]);
								selectedTexture->fullpath = selection[0];
								selectedTexture->load();
								selectedTexture->name = selection[0];
							}
							
						}
					}
				}
				ImGui::End();
				if (opentexture == false)
				{
					selectedTexture = nullptr;
				}
			}
		}

		
	}
	ImGui::End();
}

bool Editor::DrawSingleProjectItem(void * image, std::string name, int n, int buttons_count, bool *isClicked)
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
		if (isClicked)
			*isClicked = ImGui::IsItemClicked();
	}
	float last_button_x2 = ImGui::GetItemRectMax().x;
	float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
	if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
		ImGui::SameLine();
	ImGui::PopID();

	return isHovered;
}
bool Editor::DrawSingleProjectItem(glm::vec3 color, std::string name, int n, int buttons_count)
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
			ImGui::PushStyleColor(ImGuiCol_Button, { color.x,color.y,color.z,1.0 });
			ImGui::Button(("##empty"+name).c_str(),ImVec2(button_sz.x - 4, button_sz.y * 6 / 8));
			ImGui::PopStyleColor();
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



