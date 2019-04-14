#include "projectselect.h"

#include<editor/ProjectManager.h>
#include<editor/Serializable.h>
projectselect::projectselect()
{
}


projectselect::~projectselect()
{
}

void projectselect::Render()
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

	

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	 	
	ImGui::Begin("Game Engine Start", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_None | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	{
		if (ImGui::Button("Open Project"))
		{
			auto selection = pfd::open_file("Select a project", ".",
				{ "Project Files", "*.project", }).result();
			if (!selection.empty())
			{
				projectManager = Serializable::Load(selection[0].c_str());
			}
		}

		{
			ImGui::BeginChild(1, { 0,0 }, true);

			static char namebuffer[30] = "Test Project 1";
			static char pathbuffer[255] = "C:\\Projects\\";
			ImGui::Text("Project Name      : "); ImGui::SameLine(); ImGui::InputText("###Project Name:", namebuffer, 30);
			ImGui::Text("Project Location  : "); ImGui::SameLine(); ImGui::InputText("###Project Location:", pathbuffer, 255);

			if (ImGui::Button("Create Project") && strlen(namebuffer) > 3)
			{
				projectManager = new ProjectManager(namebuffer, std::string(pathbuffer) + namebuffer);
			}

			ImGui::EndChild();
		}
	

		ImGui::End();
	}

	

	
}
