#pragma once
#include<Api.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include<file_dialog.h>
#include <imgui_user.h>
#include <imconfig.h>
class ProjectManager;

class projectselect
{
public:
	projectselect();
	~projectselect();

	ProjectManager *projectManager;

	void Render();

	inline bool isSelected(){ return (projectManager != NULL); };
};

